/*
*	Aleksandra Sikora
*	Numer indeksu: 280838
*/
#include "utils.h"

typedef struct Packet
{
	int packet_id;
	int packet_ttl;
	char *packet_ip_addr;
	time_t send_time;
} Packet;

Packet receive(int sockfd);
int get_response_time(Packet packets[3], clock_t send_time);
