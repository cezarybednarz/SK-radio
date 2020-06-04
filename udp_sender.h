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

#include "err.h"
#include "constants.h"

class Udp_sender {
    
    int sock;
    
    char* connection_addr;
    char* connection_port;
    int timeout_in_seconds;
    bool multicast;
    
    ip_mreq ipmreq;
    sockaddr_in local_addr;
    sockaddr_in remote_addr;
    
public:
    
    Udp_sender(std::string addr, std::string port, int timeout, bool multi);
    void socket_connect();
    void send_message(std::string message);
    void send_message_direct(std::string message, const sockaddr_in &dst_addr);
};

#endif /* UDP_SENDER_H */
