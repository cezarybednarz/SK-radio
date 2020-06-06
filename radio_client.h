#ifndef RADIO_CLIENT_H
#define RADIO_CLIENT_H

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
#include <ctime>

#include "err.h"
#include "udp_socket.h"

class Radio_client {

    unsigned flags;
    std::string host;
    std::string udp_port;
    std::string tcp_port;
    int timeout;

public:
    Radio_client();
    bool init(int argc, char *argv[]);
    void start();

    bool parse_host(const std::string& host);
    bool parse_udp_port(const std::string& resource);
    bool parse_tcp_port(const std::string& port);
    bool parse_timeout(const std::string& metadata);
};

#endif //RADIO_CLIENT_H
