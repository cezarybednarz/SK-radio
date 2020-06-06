#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include <atomic>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "err.h"
#include "constants.h"

class Udp_socket {
    
    int sock;

    char* connection_port;
    char* connection_multi;
    int timeout_in_seconds;
    bool multicast;


    char buffer[BSIZE];
    struct ip_mreq ip_mreq;
    sockaddr_in local_addr;

public:
    
    Udp_socket(std::string port, std::string multi, int timeout);
    void socket_connect();
    std::pair<sockaddr, socklen_t> receive_message();
    void send_message_direct(std::string message, sockaddr &dst_addr, socklen_t addrlen);
    std::string get_buffer(); /* message recieved from last receive_message() */
    static std::string create_datagram(uint16_t type, uint16_t length, std::string message);
    static std::tuple<uint16_t, uint16_t, std::string> read_datagram(std::string);
    int get_socket();

};

#endif /* UDP_SENDER_H */
