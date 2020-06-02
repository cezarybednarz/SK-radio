#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>

#include "constants.h"
#include "tcp_socket.h"
#include "err.h"

#ifndef RADIO_PROXY_H
#define RADIO_PROXY_H


class Radio_proxy {
    
    unsigned flags;
    std::string host;
    std::string resource;
    std::string port;
    bool metadata;
    int timeout;
    
public:
    Radio_proxy();
    bool init(int argc, char* argv[]);
    void start();
    std::string create_get_request();

private:
    bool parse_host(const std::string& host);
    bool parse_resource(const std::string& resource);
    bool parse_port(const std::string& port);
    bool parse_metadata(const std::string& metadata);
    bool parse_timeout(const std::string& timeout);
};

#endif /* RADIO_PROXY_H */
