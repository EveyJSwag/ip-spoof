#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <err.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include "../lib/util.h"
int main()
{
    
    //MAKE SOCKET
    const char* targetIP = "192.168.7.50";
    unsigned int targetPort = 123;
    unsigned int protocol = IPPROTO_UDP;
    int socketFD = socket(AF_INET, SOCK_RAW, protocol);
    
    struct sockaddr_in serverSocket;
    serverSocket.sin_addr.s_addr = inet_addr(targetIP);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(50000);
    
    int connectResult = connect(
        socketFD, 
        (struct sockaddr*)&serverSocket,
        sizeof(struct sockaddr)
    );
    
 
    
    struct udphdr* udph = (struct udphdr*)malloc(sizeof(struct udphdr));
    struct iphdr* iph = (struct iphdr*)malloc(sizeof(struct iphdr));
    udpsuhdr* udpsuh = (udpsuhdr*) malloc(sizeof(udpsuhdr)); 
    
    char datagram[4096]; 
    char* data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    char spoofMessage[48] = {0x45,0x45};
    memcpy(data, spoofMessage, strlen(spoofMessage));
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 90;
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("127.0.0.1");
    iph->daddr = serverSocket.sin_addr.s_addr;
    iph->check = checkSum((unsigned short*)iph, iph->tot_len);

    udph->dest = serverSocket.sin_port;
    udph->source = htons(212);
    udph->check = 0;
    udph->len = htons(strlen(data) + sizeof(struct udphdr));

    udpsuh->srcAddr = inet_addr("192.168.7.33");
    udpsuh->destAddr = serverSocket.sin_addr.s_addr;
    udpsuh->protocol = protocol;
    udpsuh->totalLength = sizeof(struct udphdr) + strlen(data);

    char* pIPplusUDPplusData = malloc(sizeof(udpsuhdr) + sizeof(struct udphdr)+ strlen(data));
    memcpy(pIPplusUDPplusData, udpsuh, sizeof(udpsuhdr));
    memcpy(pIPplusUDPplusData + sizeof(udpsuhdr), udph, sizeof(struct udphdr));
    memcpy(pIPplusUDPplusData + sizeof(udpsuhdr) +sizeof(struct udphdr), data, strlen(data));


    udph->check = checkSum((unsigned short*)pIPplusUDPplusData, sizeof(udpsuhdr) + sizeof(struct udphdr)+ strlen(data));

    memcpy(datagram, iph, sizeof(struct iphdr));
    memcpy(datagram + sizeof(struct iphdr), udph, sizeof(struct udphdr));
    memcpy(datagram + sizeof(struct iphdr) + sizeof(struct udphdr), data, strlen(data));
    
    int on = 1;
    const int* onPtr = &on;
    
    int setSockOptResult = setsockopt(socketFD, IPPROTO_IP, IP_HDRINCL, onPtr, sizeof(on));

    int sendRES = send(socketFD, datagram, iph->tot_len, 0);
    
    printf("%d\n", sendRES);

    return 0;
}