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
#include <time.h>

//char serverAddress[] = "challenge.drl.io";
#define PORT  8162
using namespace std;

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

uint64_t Protocol::timestamp(uint8_t * buffer)
{
    m_timestamp = *(buffer+3);
    uint32_t counter = 0;
  
    counter  =  ((m_timestamp & 0xC0) >> 6);

    if (counter >= 0x3FFFF || counter < m_timestamp){m_timestamp+=counter;}
    else {m_timestamp = counter;}

    return m_timestamp;
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
    printf("Enter the Message: ");
    fgets(sendToServer, 100, stdin);

    //Send data to the server
    pProtocol->socketSend(hSocket, sendToServer, strlen(sendToServer));
    int read_size = pProtocol->socketRecieve(hSocket, server_reply, 200);
    printf("Server Response : %s: read_size %d\n\n",server_reply, read_size);
    close(hSocket);
    return 0;
}