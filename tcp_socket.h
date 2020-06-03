#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

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
#include <poll.h>

#include "err.h"

class Tcp_socket {

    int sock;
    char* connection_addr;
    char* connection_port;
    FILE *fp;

public:
    Tcp_socket(const std::string& addr, const std::string& port);
    void socket_connect();
    void socket_send_request(std::string content);
    std::string socket_getline();
    std::string socket_read_n_bytes(size_t n);
    char read_char();
};

#endif //TCP_SOCKET_H
