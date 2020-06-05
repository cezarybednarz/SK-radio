#include "radio_client.h"

bool Radio_client::parse_host(const std::string& _host) {
    if (flags & CLIENT_HOST_DEFINED)
        return false;
    flags |= CLIENT_HOST_DEFINED;

    // parsowanie

    host = _host;

    return true;
}

bool Radio_client::parse_udp_port(const std::string& _udp_port) {
    if (flags & CLIENT_UDP_PORT_DEFINED)
        return false;
    flags |= CLIENT_UDP_PORT_DEFINED;

    for (char c : _udp_port) {
        if (!std::isdigit(c))
            return false;
    }

    udp_port = _udp_port;

    return true;
}

bool Radio_client::parse_tcp_port(const std::string& _tcp_port) {
    if (flags & CLIENT_TCP_PORT_DEFINED)
        return false;
    flags |= CLIENT_TCP_PORT_DEFINED;

    for (char c : _tcp_port) {
        if (!std::isdigit(c))
            return false;
    }

    tcp_port = _tcp_port;

    return true;
}

bool Radio_client::parse_timeout(const std::string& _timeout) {
    if (flags & CLIENT_TIMEOUT_DEFINED)
        return false;
    flags |= CLIENT_TIMEOUT_DEFINED;

    for (char c : _timeout) {
        if (!std::isdigit(c))
            return false;
    }

    timeout = std::stoi(_timeout);

    return true;
}

bool Radio_client::init(int argc, char *argv[]) {
    if ((argc % 2) != 1)
        return false;

    for (int i = 1; i + 1 < argc; i += 2) {
        std::string curr = argv[i], next = argv[i + 1];
        if (curr == "-H") {
            if (!parse_host(next))
                return false;
        }
        else if (curr == "-P") {
            if (!parse_udp_port(next))
                return false;
        }
        else if (curr == "-p") {
            if(!parse_tcp_port(next))
                return false;
        }
        else if (curr == "-T") {
            if (!parse_timeout(next))
                return false;
        }
        else {
            std::cerr << "wrong flag\n";
            return false;
        }
    }

    if (!(flags & CLIENT_HOST_DEFINED)) {
        std::cerr << "define host\n";
        return false;
    }

    if (!(flags & CLIENT_UDP_PORT_DEFINED)) {
        std::cerr << "define udp port\n";
        return false;
    }

    if (!(flags & CLIENT_TCP_PORT_DEFINED)) {
        std::cerr << "define tcp port\n";
        return false;
    }

    return true;
}

Radio_client::Radio_client() {
    /* default values */
    flags = 0;
    timeout = 5;
}

void Radio_client::start() {
    std::cout << "Starting radio-client\n";

    Udp_socket udp_socket;




    std::cout << "radio-client started...\n";

    while(errno >= 0) {

    }
}

void send_discover() {

}

void send_keepalive() {

}

int main(int argc, char* argv[]) {
    Radio_client radio_client;
    if (!radio_client.init(argc, argv)) {
        syserr("Radio_client::init()")
    }
    else {
        radio_client.start();
    }
    return 0;
}