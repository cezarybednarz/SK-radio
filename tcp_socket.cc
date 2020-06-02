
#include "tcp_socket.h"

Tcp_socket::Tcp_socket(char *addr, char *port) : connection_addr(addr), connection_port(port)
{ }

void Tcp_socket::socket_connect() {
    struct addrinfo addr_hints, *addr_result;

    std::memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; /* IPv4 */
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;

    int err = getaddrinfo(connection_addr, connection_port, &addr_hints, &addr_result);
    if (err == EAI_SYSTEM) /* system error */
        syserr("getaddrinfo: %s", gai_strerror(err));
    else if (err != 0) // other error (host not found, etc.)
        fatal("getaddrinfo: %s", gai_strerror(err));

    // initialize socket according to getaddrinfo results
    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0)
        syserr("socket");

    // connect socket to the server
    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
        syserr("connect");

    freeaddrinfo(addr_result);
}

void Tcp_socket::socket_send(std::string content) {
    
}

std::string Tcp_socket::socket_receive() {

    return "";
}
