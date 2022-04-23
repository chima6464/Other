#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "protocol.h"
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <inttypes.h>
#include "sys/time.h"

//char serverAddress[] = "challenge.drl.io";
#define PORT  8162
#define MAX_SEND_LEN 64
using namespace std;
static int sockfd = -1, connfd = 0;
static pthread_mutex_t wifiDataMutex = PTHREAD_MUTEX_INITIALIZER; // used for txbuf and socket

Protocol::Protocol(): m_magic(0xf00d),m_length(0),m_message_type(0),m_timestamp(0),
m_counter(0), m_payload_cs(0), m_header_cs(0), m_payload(0)
{
}

Protocol::~Protocol()
{
 

}

uint16_t Protocol::magic(uint8_t * buffer)
{
  m_magic = *(buffer);
  return m_magic; 
}

void Protocol::lengthOfMessage(uint8_t * buffer)
{
  m_length = (sizeof(buffer))/(sizeof(buffer[0]));
}

uint8_t Protocol::messageType(uint8_t * buffer)
{
  m_message_type = *(buffer + 2);
  return m_message_type;
}

uint64_t Protocol::timestamp(void)
{
  struct timeval currTime;
  time_t T = time(NULL);
  struct tm tm = *localtime(&T);
  uint16_t time_ms = 0;
  uint64_t total_time_ms;

  gettimeofday(&currTime, NULL);
  time_ms = currTime.tv_usec / 1000;
  total_time_ms = tm.tm_hour * 3600 * 1000 + tm.tm_min * 60 * 1000 + tm.tm_sec * 1000 + time_ms;
  return total_time_ms;  
}

uint8_t Protocol::counterFromClient(void)
{
  return m_counter++;
}

short Protocol::socketCreate(void)
{
  short hSocket;
  std::cout << "Create the Socket " << std::endl;
  hSocket = socket(AF_INET, SOCK_STREAM, 0);
  return hSocket;
}

int Protocol::socketConnect(int hSocket)
{
    int iRetval=-1;
    struct sockaddr_in remote= {0};
    //Couldn't reach "challenge.drl.io", so I pinged it via terminal and this 
    //IP address showed up
    remote.sin_addr.s_addr = inet_addr("3.91.129.142"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(8162);
    iRetval = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
    return iRetval;
}

int Protocol::socketSend(int hSocket, char * buffer, short lenRqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;

    if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0) {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket, buffer, lenRqst, 0);
    return shortRetval;
}

int Protocol::socketRecieve(int hSocket,char* Rsp,short RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;

    if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0)    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = recv(hSocket, Rsp, RvcSize, 0);
    printf("Response %s\n",Rsp);
    return shortRetval;
}


uint16_t Protocol::checksum(uint8_t * buffer, uint16_t length)
{
    int x = 1;
    int counter = 0;
    for (size_t i = 0; i < length; i++)
    {
        /* code */
        x= (x + counter + *(buffer + i)) % 65521;
        counter = (counter + 1) % 256;
    }
    m_payload_cs = x;
    return x;
    
}

int Protocol::sendInfo(int hSocket)
{
  uint8_t txBuf[MAX_SEND_LEN] = {0};
  m_length = sizeof(SoftwareCommandDroneInfo_t);
  int rc = 0;
  SoftwareCommandDroneInfo_t * const responsePacket = (SoftwareCommandDroneInfo_t *)txBuf;
  responsePacket->s_message_type = eClientRequest;
  responsePacket->s_timestamp = timestamp();
  responsePacket->s_counter++;

  pthread_mutex_lock(&wifiDataMutex);
  size_t const headerAndPayloadAndCrcLength__bytes = sizeof(SoftwareCommandDroneInfo_t) + m_length;
  txBuf[headerAndPayloadAndCrcLength__bytes] = checksum(txBuf, headerAndPayloadAndCrcLength__bytes);
  rc = send(hSocket, txBuf, headerAndPayloadAndCrcLength__bytes, 0);
  pthread_mutex_unlock(&wifiDataMutex); //un lock
  return rc;

}

int main(int argc, char *argv[])  {
    int hSocket = 0;

    char sendToServer[100] = {0};
    char server_reply[200] = {0};

    Protocol * pProtocol = new Protocol();

    //Create socket
    hSocket = pProtocol->socketCreate();

    if(hSocket == -1)   {
        printf("Could not create socket\n");
        return 1;
    }

    printf("Socket is created\n");

    //Connect to remote server
    int sckConn = pProtocol->socketConnect(hSocket);
    if (sckConn < 0) {
        perror("connect failed.\n");
        return 1;
    }

    printf("Sucessfully conected with server\n");
    //printf("Enter the Message: ");
    //fgets(sendToServer, 100, stdin);

    //Send data to the server
    pProtocol->sendInfo(hSocket);
    int read_size = pProtocol->socketRecieve(hSocket, server_reply, 200);
    printf("Server Response : %s: read_size %d\n\n",server_reply, read_size);
    close(hSocket);
    return 0;
}