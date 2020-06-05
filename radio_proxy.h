#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <map>
#include <cctype>
#include <ctime>
#include <cerrno>
#include <poll.h>

#include "constants.h"
#include "tcp_socket.h"
#include "err.h"
#include "udp_socket.h"

#ifndef RADIO_PROXY_H
#define RADIO_PROXY_H


class Radio_proxy {

    unsigned flags;

    /* A */
    std::string host;
    std::string resource;
    std::string port;
    bool metadata;
    int timeout;

    /* A+B */
    bool udp_flags;
    std::string udp_port;
    std::string udp_multicast;
    int udp_timeout;

    std::string first_line_of_response;
    std::map<std::string, std::string> header_info;
    int icy_metaint; /* '-1' if no icy-metaint defined in response (or if icy-metaint is equal 0) */

public:
    Radio_proxy();
    bool init(int argc, char* argv[]);
    void start();

private:
    bool parse_host(const std::string& host);
    bool parse_resource(const std::string& resource);
    bool parse_port(const std::string& port);
    bool parse_metadata(const std::string& metadata);
    bool parse_timeout(const std::string& timeout);
    bool parse_udp_port(const std::string& udp_port);
    bool parse_udp_multicast(const std::string& udp_multicast);
    bool parse_udp_timeout(const std::string& udp_timeout);

    void no_udp_casting(Tcp_socket &tcp_socket); /* main A loop */
    void udp_casting(Tcp_socket &tcp_socket); /* main A+B loop */

    std::string create_get_request();
    void read_header(Tcp_socket &tcp_socket);
    std::string read_data(Tcp_socket &tcp_socket);
    std::string read_metadata(Tcp_socket &tcp_socket);
    std::string read_continuous_data(Tcp_socket &tcp_socket, size_t buffer);

};

#endif /* RADIO_PROXY_H */
