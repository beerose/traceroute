/*
*	Aleksandra Sikora
*	Numer indeksu: 280838
*/
#include "receive.h"

/* everage response time from three receiced packets */
int get_response_time(Packet packets[3], clock_t send_time)
{
	int sum = (packets[0].send_time - send_time);
	sum += (packets[1].send_time - send_time);
	sum += (packets[2].send_time - send_time);

	return sum / 3;
}

Packet receive(int sockfd)
{
	Packet packet = {-1, -1, NULL, clock()};

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];

	/* MSG_DONTWAIT for non blocking mode */
	ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr *)&sender, &sender_len);
	if (packet_len < 0)
	{
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
		return packet;
	}
	char sender_ip_str[20];
	inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

	/* getting to icmp_header */
	struct iphdr *ip_header = (struct iphdr *)buffer;
	struct icmphdr *icmp_header = (struct icmphdr *)(buffer + 4 * ip_header->ihl);

	if (icmp_header->type == ICMP_ECHOREPLY)
	{
		clock_t now = clock();
		packet.packet_id = icmp_header->un.echo.id;
		packet.packet_ttl = icmp_header->un.echo.sequence / 3 + 1; /* division by number of packets to get TTL */
		packet.packet_ip_addr = sender_ip_str;
		packet.send_time = now;
		return packet;
	}

	if (icmp_header->type == ICMP_TIME_EXCEEDED)
	{
		/* 
			icmp_header:
			8 bits <- type
			8 bits <- code; time exceeded type
			16 bits <- checksum
			32 bits <- nothing
			32 + 64 <- header and data; from 64
		*/
		u_int8_t *icmp_packet = buffer + 4 * ip_header->ihl + 8; /* ip header length */
		icmp_packet += 4 * ((struct ip *)(icmp_packet))->ip_hl;
		struct icmp *icmp_time_ex = (struct icmp *)icmp_packet;

		clock_t now = clock();
		packet.packet_id = icmp_time_ex->icmp_id;
		packet.packet_ttl = icmp_time_ex->icmp_seq / 3 + 1; /* division by number of packets to get TTL */
		packet.packet_ip_addr = sender_ip_str;
		packet.send_time = now;
		return packet;
	}
	return packet;
}
