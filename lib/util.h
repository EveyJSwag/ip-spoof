#ifndef UTIL_S
#define UTIL_S
unsigned short checkSum(unsigned short *dataPtr,int lenBytes);
typedef struct udpsudhdr
{
    unsigned int srcAddr;
    unsigned int destAddr;
    const unsigned char padding;
    unsigned char protocol;
    unsigned short totalLength;
} udpsuhdr;
#endif