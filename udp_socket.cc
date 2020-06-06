#include "udp_socket.h"


Udp_socket::Udp_socket(std::string port, std::string multi, int timeout)
 : connection_port(const_cast<char*>(port.c_str())), connection_multi(const_cast<char*>(multi.c_str())),
   timeout_in_seconds(timeout) {
    multicast = (multi != "");
}
   

void Udp_socket::socket_connect() {
    /* get socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)   
        syserr("socket");

    /* activating broadcasting */
    int optval = 1;
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



std::pair <sockaddr, socklen_t> Udp_socket::receive_message() {
    struct sockaddr src_addr;
    socklen_t addrlen;
    memset(buffer, 0, BSIZE);
    if (recvfrom(sock, &buffer, BSIZE, 0, &src_addr, &addrlen) < 0)
        syserr("recvfrom");
    return std::make_pair(src_addr, addrlen);
}

void Udp_socket::send_message_direct(std::string message, sockaddr &dst_addr, socklen_t addrlen) {
    //memset(buffer, 0, BSIZE);
    for(size_t i = 0; i < message.length(); i++)
        buffer[i] = message[i];
    if (sendto(sock, buffer, message.length(), 0, &dst_addr, addrlen) < 0)
        syserr("sendto");
}

std::string Udp_socket::get_buffer() {
    std::string ret;
    for(int i = 0; i < BSIZE; i++) {
        ret.push_back(buffer[i]);
    }
    return ret;
}

std::string Udp_socket::create_datagram(uint16_t type, uint16_t length, std::string message) {
    std::string ret(4 + message.length(), 0);
    type = htons(type);
    length = htons(length);
    ret[0] = (char)(type % OCTET);
    ret[1] = (char)(type / OCTET);
    ret[2] = (char)(length % OCTET);
    ret[3] = (char)(length / OCTET);
    for (size_t i = 0; i < message.length(); i++) {
        ret[i + 4] = message[i];
    }
    return ret;
}

std::tuple<uint16_t, uint16_t, std::string> Udp_socket::read_datagram(std::string data) {
    uint16_t type = ntohs(data[1] * OCTET  + data[0]);
    uint16_t length = ntohs(data[3] * OCTET + data[2]);
    std::string message;
    for (size_t i = 4; i < 4 + (unsigned)length; i++) {
        message.push_back(data[i]);
    }
    return std::make_tuple(type, length, message);
}
int Udp_socket::get_socket() {
    return sock;
}
