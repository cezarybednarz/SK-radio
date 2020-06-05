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
    sockaddr_in remote_addr;
    
public:
    
    Udp_socket(std::string port, std::string multi, int timeout);
    void socket_connect();
    void receive_message();
    void send_message(std::string message);
    void send_message_direct(std::string message, const sockaddr_in &dst_addr, socklen_t addrlen);

    char *get_buffer();
};

#endif /* UDP_SENDER_H */
