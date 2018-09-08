#ifndef _LAN_PLAY_H_
#define _LAN_PLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pcap.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <base/llog.h>
#include <uv.h>

struct lan_play;
#include "packet.h"
#include "helper.h"
#include "config.h"
#include "arp.h"
#include "proxy.h"

struct lan_play {
    pcap_t *dev;
    uint32_t id;
    void *buffer;
    uint8_t ip[4];
    uint8_t subnet_net[4];
    uint8_t subnet_mask[4];
    uint8_t mac[6];
    uint16_t identification;
    struct arp_item arp_list[ARP_CACHE_LEN];
    time_t arp_ttl;
    bool stop;
    uv_loop_t loop;
    uv_thread_t libpcap_thread;
    uv_async_t get_packet_handle;

    // lan_client
    uv_udp_t client;
    uv_timer_t client_keepalive_timer;
    struct sockaddr_in server_addr;

    struct proxy proxy;
};

void get_packet(struct lan_play *arg, const struct pcap_pkthdr * pkthdr, const u_char * packet);
int send_packet(struct lan_play *arg, int size);
int process_arp(struct lan_play *arg, const struct ether_frame *ether);
int process_ipv4(struct lan_play *arg, const struct ether_frame *ether);
void lan_client_init(struct lan_play *lan_play);
int lan_client_send_ipv4(struct lan_play *lan_play, void *dst_ip, const void *packet, uint16_t len);

#endif // _LAN_PLAY_H_
