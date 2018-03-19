/*
*	Aleksandra Sikora
*	Numer indeksu: 280838
*/
#include "send.h"

/*
    source of majority of below functions: http://www.ii.uni.wroc.pl/~mbi/dyd/sieci_18s/lec2.pdf
*/

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
    u_int32_t sum;
    const u_int16_t *ptr = buff;
    assert(length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}

struct icmphdr make_header(int echo_sequence, pid_t echo_id)
{
    struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = echo_id;
    icmp_header.un.echo.sequence = echo_sequence;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum((u_int16_t *)&icmp_header, sizeof(icmp_header));

    return icmp_header;
}

struct sockaddr_in make_recipient(char *dest_ip)
{
    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, dest_ip, &recipient.sin_addr);

    return recipient;
}

int send_packet(int TTL, const int sockfd, struct sockaddr_in recipient, int sequence, int id)
{
    /*
        Pole TTL jest w nagłówku IP → brak bezpośredniego dostępu. Zmiana wywołaniem:
    */
    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &TTL, sizeof(int));

    struct icmphdr icmp_header;
    icmp_header = make_header(sequence, id);
    ssize_t bytes_sent = sendto(sockfd, &icmp_header, sizeof(icmp_header), 0, (struct sockaddr *)&recipient, sizeof(recipient));
    if (bytes_sent == -1)
    {
        return -1;
    };
    return 0;
}
