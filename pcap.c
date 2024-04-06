#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>

void process_packet(const u_char *packet) {
    struct ethhdr *eth = (struct ethhdr *)packet;
    struct iphdr *iph = (struct iphdr *)(packet + ETH_HLEN);
    struct tcphdr *tcph = (struct tcphdr *)(packet + iph->ihl*4 + ETH_HLEN);

    printf("Source MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("Destination MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);

    printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
    printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));

    printf("Source Port: %u\n", ntohs(tcph->source));
    printf("Destination Port: %u\n", ntohs(tcph->dest));

    printf("Message: %s\n", packet + iph->ihl*4 + tcph->doff*4 + ETH_HLEN);
}

int main() {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    const u_char *packet;

    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", "eth0", errbuf);
        return(2);
    }

    while (1) {
        packet = pcap_next(handle, &header);
        process_packet(packet);
    }

    pcap_close(handle);
    return(0);
}
