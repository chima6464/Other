#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

class Protocol
{
private:
    /* data */
    uint16_t m_magic;
    uint16_t m_length;
    uint8_t m_message_type;
    uint64_t m_timestamp;
    uint8_t m_counter;
    uint16_t m_payload_cs;
    uint16_t m_header_cs;
    uint64_t m_payload;

    
public:

    enum MessageType
    {
        eClientRequest,
        eServerReply
    };
    Protocol(/* args */);
    virtual ~Protocol();
    uint16_t magic(uint8_t * buffer);
    void lengthOfMessage(uint8_t * buffer);
    uint8_t messageType(uint8_t * buffer);
    uint64_t timestamp(uint8_t * buffer);
    uint8_t counterFromClient(void);
    uint16_t payloadCRC(uint8_t * buffer);
    uint16_t HeaderCRC(uint8_t * buffer);
    short socketCreate(void);
    int socketConnect(int hSocket);
    int socketSend(int hSocket, char * buffer, short lenRqst);
    int socketRecieve(int hSocket,char* Rsp,short RvcSize);
    uint16_t checksum(uint8_t * buffer, uint16_t length);
};

