#include "udp_socket.h"


Udp_socket::Udp_socket(std::string port, std::string multi, int timeout)
 : connection_port(const_cast<char*>(port.c_str())), connection_multi(const_cast<char*>(multi.c_str())),
   timeout_in_seconds(timeout)
{
    if (multi != "") {
        multicast = true;
    }
    else {
        multicast = false;
    }
}
   

void Udp_socket::socket_connect() {
    /* get socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)   
        syserr("socket");
    
    /* set reusing address */
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0)
        syserr("setsockopt (SO_REUSEADDR");
    
    /* set reusing port */
    optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval) < 0)
        syserr("setsockopt (SO_REUSEPORT)");

    /* activating broadcasting */
    optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &optval, sizeof optval) < 0)
        syserr("setsockopt (SO_BROADCAST)");

    /* activate multicast sending */
    if (multicast) {
        /* podłączenie do grupy rozsyłania (ang. multicast) */
        ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (inet_aton(connection_multi, &ip_mreq.imr_multiaddr) == 0) {
            fprintf(stderr, "ERROR: inet_aton - invalid multicast address\n");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &ip_mreq, sizeof ip_mreq) < 0)
            syserr("setsockopt");

        /* setting TTL for group datagrams */
        optval = TTL_VALUE;
        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &optval, sizeof optval) < 0)
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



void Udp_socket::receive_message() {
    struct sockaddr src_addr;
    socklen_t addrlen;
    memset(buffer, 0, BSIZE);
    if (recvfrom(sock, &buffer, BSIZE, 0, &src_addr, &addrlen) < 0)
        syserr("recvfrom");

    printf("Request from: %s\n", inet_ntoa(((struct sockaddr_in *) &src_addr)->sin_addr));

    printf("otrzymałem %s", buffer);
}

void Udp_socket::send_message(std::string message) {
}

void Udp_socket::send_message_direct(std::string message, const sockaddr_in &dst_addr, socklen_t addrlen) { // todo nieprzetestowane
    memset(buffer, 0, BSIZE);
    strncpy(buffer, message.c_str(), BSIZE);
    size_t length = strnlen(buffer, BSIZE);
    if (sendto(sock, buffer, length, 0, reinterpret_cast<const sockaddr*>(&dst_addr), addrlen) < 0)
        syserr("sendto");
}

char* Udp_socket::get_buffer() {
    return buffer;
}
