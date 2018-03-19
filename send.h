/*
*	Aleksandra Sikora
*	Numer indeksu: 280838
*/
#include "utils.h"

u_int16_t compute_icmp_checksum(const void *buff, int length);
struct icmphdr make_header(int echo_sequence, pid_t echo_id);
struct sockaddr_in make_recipient(char *dest_ip);
int send_packet(int TTL, const int sockfd, struct sockaddr_in recipient, int sequence, int id);
