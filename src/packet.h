#ifndef IPTRAF_NG_PACKET_H
#define IPTRAF_NG_PACKET_H

/***

packet.h - external declarations for packet.c

***/

#include "fltselect.h"

/*
 * Number of bytes from captured packet to move into a buffer.
 * 96 bytes should be enough for the IP header, TCP/UDP/ICMP/whatever header
 * with reasonable numbers of options.
 */
#define MAX_PACKET_SIZE 96

#define INVALID_PACKET 0
#define PACKET_OK 1
#define CHECKSUM_ERROR 2
#define PACKET_FILTERED 3
#define MORE_FRAGMENTS 4

struct pkt_hdr {
	size_t		pkt_bufsize;
	char	       *pkt_payload;
	size_t		pkt_caplen;	/* bytes captured */
	size_t		pkt_len;	/* bytes on-the-wire */
	int		pkt_ifindex;	/* Interface number */
	unsigned short	pkt_protocol;	/* Physical layer protocol: ETH_P_* */
	unsigned short	pkt_hatype;	/* Header type: ARPHRD_* */
	unsigned char	pkt_pkttype;	/* Packet type: PACKET_OUTGOING, PACKET_BROADCAST, ... */
	unsigned char	pkt_halen;	/* Length of address */
	unsigned char	pkt_addr[8];	/* Physical layer address */
	struct ethhdr  *ethhdr;
	struct fddihdr *fddihdr;
	struct iphdr   *iphdr;
	struct ip6_hdr *ip6_hdr;
	char		pkt_buf[MAX_PACKET_SIZE];
};

static inline void PACKET_INIT_STRUCT(struct pkt_hdr *p)
{
	p->pkt_bufsize = MAX_PACKET_SIZE;
	p->pkt_payload = NULL;
	p->ethhdr      = NULL;
	p->fddihdr     = NULL;
	p->iphdr       = NULL;
	p->ip6_hdr     = NULL;
	p->pkt_len     = 0;             /* signalize we have no packet prepared */
}

#define PACKET_INIT(packet)					\
	struct pkt_hdr packet;					\
	PACKET_INIT_STRUCT(&packet)

void open_socket(int *fd);
int packet_get(int fd, struct pkt_hdr *pkt, int *ch, WINDOW *win);
int packet_process(struct pkt_hdr *pkt, unsigned int *total_br,
		   unsigned int *sport, unsigned int *dport,
		   struct filterstate *filter, int match_opposite,
		   int v6inv4asv6);
void pkt_cleanup(void);

#endif	/* IPTRAF_NG_PACKET_H */
