#include "udp_sender.h"


Udp_sender::Udp_sender(std::string addr, std::string port, int timeout, bool multi) 
 : connection_addr(const_cast<char*>(addr.c_str())), connection_port(const_cast<char*>(port.c_str())),
   timeout_in_seconds(timeout), multicast(multi)
{ }
   

void Udp_sender::socket_connect() {
    /* get socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)   
        syserr("socket");
    
    /* set reusing address */
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0)
        syserr("setsockopt (SO_REUSEADDR");
    
    /* set reusing port */
    int optval_port = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval_port, sizeof optval_port) < 0)
        syserr("setsockopt (SO_REUSEPORT)");
    
    /* activate multicast sending */
    if (multicast) {
        /* activating broadcasting */
        int optval_broadcast = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &optval_broadcast, sizeof optval_broadcast) < 0)
            syserr("setsockopt (SO_BROADCAST)");

        /* setting TTL for group datagrams */
        int optval_ttl = TTL_VALUE;
        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &optval_ttl, sizeof optval_ttl) < 0)
            syserr("setsockopt (IP_MULTICAST_TTL)");
    }
    
    /* bind local port */
    in_port_t port = std::stoi(connection_port);
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *) &local_addr, sizeof(local_addr)) < 0)
        syserr("bind");
}

void Udp_sender::send_message(std::string message) {
    
}

void Udp_sender::send_message_direct(std::string message, const sockaddr_in &dst_addr) {
    
}
