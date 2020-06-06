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

    Udp_socket udp_socket(udp_port, "", timeout);
    udp_socket.socket_connect();

    std::cout << "radio-client started...\n";

    /* convert std::string host to sockaddr proxy_sockaddr */
    struct sockaddr_in proxy_addr;
    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_port = htons(std::stoi(udp_port));
    inet_pton(AF_INET, host.c_str(), &proxy_addr.sin_addr);
    int proxy_addrlen = sizeof(struct sockaddr_in);
    struct sockaddr *proxy_sockaddr = reinterpret_cast<sockaddr*>(&proxy_addr);

    /* send DISCOVER */
    std::cout << "sending DISCOVER\n";
    udp_socket.send_message_direct(Udp_socket::create_datagram(DISCOVER, 0, ""), *proxy_sockaddr, proxy_addrlen);


    struct pollfd group[1];

    /* initialise UDP poll group */
    group[0].fd = udp_socket.get_socket();
    group[0].events = (POLLIN | POLLOUT | POLLERR);
    group[0].revents = 0;

    std::clock_t last_keepalive = std::clock();

    while (errno >= 0) {
        group[0].revents = 0;

        int ret = poll(group, 1, 1000 * timeout);
        if (ret < 0)
            syserr("poll");
        if (ret == 0)
            syserr("poll timeout");

        /* reading data from UDP socket */
        if (group[0].revents & POLLIN) {
            auto addr_pair = udp_socket.receive_message();
            auto data = Udp_socket::read_datagram(udp_socket.get_buffer());

            std::cout << "received data: " << std::get<0>(data) << " " << std::get<1>(data) << " [" << std::get<2>(data) << "] " << "from " << inet_ntoa(((struct sockaddr_in *) &addr_pair.first)->sin_addr);
        }

        /* sending data to UDP socket */
        if (group[0].revents & POLLOUT) {
            /* sending KEEPALIVE if needed */
            std::clock_t curr_time = std::clock();
            if ((long double)(curr_time - last_keepalive)/(long double)CLOCKS_PER_SEC > THREE_AND_HALF_USECONDS) {
                std::cout << "sending KEEPALIVE\n";
                udp_socket.send_message_direct(Udp_socket::create_datagram(KEEPALIVE, 0, ""), *proxy_sockaddr, proxy_addrlen);
                last_keepalive = curr_time;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    Radio_client radio_client;
    if (!radio_client.init(argc, argv)) {
        syserr("Radio_client::init()");
    }
    else {
        radio_client.start();
    }
    return 0;
}