/*
*	Aleksandra Sikora
*	Numer indeksu: 280838
*/

#include "receive.h"
#include "send.h"
#include "utils.h"

int is_valid_ip(char *ip_address)
{
	unsigned long ip = 0;
	return 0 == inet_pton(AF_INET, ip_address, &ip);
}

int validate_args(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Invalid parameters input.\n");
		return false;
	}
	if (is_valid_ip(argv[1]))
	{
		fprintf(stderr, "Invalid IP address.\n");
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	if (!validate_args(argc, argv))
	{
		return 1;
	}

	int dest_reached = 0;
	int seq_num = 0; /* sequence number in packets headers */
	pid_t pID = getpid();

	char *target_ip = argv[1];

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
	{
		fprintf(stderr, "Cannot open socket. Did you run the program with sudo?");
	}

	struct sockaddr_in recipient = make_recipient(target_ip);

	fprintf(stderr, "Trace to: %s \n", target_ip);
	for (int TTL = 1; TTL <= 30 && !dest_reached; TTL++)
	{
		/* sending three ICMP echo request packets with current TTL */
		for (int i = 0; i < 3; i++)
		{
			int res = send_packet(TTL, sockfd, recipient, seq_num++, pID);
			if (res == -1)
			{
				fprintf(stderr, "Cannot send request for TTL: %d\n", TTL);
				break;
			}
		};

		int counter = 0;
		Packet received_packets[3];

		clock_t start = clock();

		fd_set descriptors;
		FD_ZERO(&descriptors);
		FD_SET(sockfd, &descriptors);
		struct timeval tv = {1, 0};

		while (counter < 3)
		{
			int ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);

			if (ready < 0)
			{
				printf("Read from socket error");
				break;
			}

			if (ready == 0)
			{
				/* end of time */
				break;
			}

			else
			{
				Packet received = receive(sockfd);
				if (received.packet_ttl == TTL && received.packet_id == pID)
				{
					/* invalid packets comes with id = -1, so no need to validate them */
					received_packets[counter++] = received;
				}
			}
		}

		/* printing route */
		if (counter == 3)
		{
			fprintf(stderr, "%d.\t%s\t%dms\n", TTL,
					received_packets[0].packet_ip_addr,
					get_response_time(received_packets, start));
			dest_reached = strcmp(received_packets[0].packet_ip_addr, target_ip) ==
						   0; /* set dest_reached as true if reached target ip */
		}
		else if (counter == 0)
		{
			fprintf(stderr, "%d.\t*\n", TTL);
		}
		else
		{
			fprintf(stderr, "%d.\t%s\t???\n", TTL,
					received_packets[0].packet_ip_addr);
		}
	}
	printf("Done! \n");
	close(sockfd);

	return 0;
}
