
#include "radio_proxy.h"
#include "udp_socket.h"

bool Radio_proxy::parse_host(const std::string& _host) {
    if (flags & HOST_DEFINED) 
        return false;
    flags |= HOST_DEFINED;
    
    // parsowanie
    
    host = _host;
    
    return true;
}

bool Radio_proxy::parse_resource(const std::string& _resource) {
    if (flags & RESOURCE_DEFINED) 
        return false;
    flags |= RESOURCE_DEFINED;
    
    // parsowanie
    
    resource = _resource;
    
    return true;
}

bool Radio_proxy::parse_port(const std::string& _port) {
    if (flags & PORT_DEFINED)
        return false;
    flags |= PORT_DEFINED;
    
    for (char c : _port) {
        if (!std::isdigit(c))
            return false;
    }

    port = _port;
    
    return true;
}

bool Radio_proxy::parse_metadata(const std::string& _metadata) {
    if (flags & METADATA_DEFINED)
        return false;
    flags |= METADATA_DEFINED;
    
    if (_metadata == "yes") {
        metadata = true;
    } 
    else if (_metadata == "no") {
        metadata = false;
    } 
    else {
        return false;
    }
    
    return true;
}

bool Radio_proxy::parse_timeout(const std::string& _timeout) {
    if (flags & TIMEOUT_DEFINED)
        return false;
    flags |= TIMEOUT_DEFINED;
    
    for (char c : _timeout) {
        if (!std::isdigit(c))
            return false;
    }
    
    timeout = std::stoi(_timeout);
    
    return true;
}

bool Radio_proxy::parse_udp_port(const std::string& _udp_port) {
    if (flags & UDP_PORT_DEFINED)
        return false;
    flags |= UDP_PORT_DEFINED;

    for (char c : _udp_port) {
        if (!std::isdigit(c))
            return false;
    }

    udp_port = _udp_port;

    return true;
}

bool Radio_proxy::parse_udp_multicast(const std::string& _udp_multicast) {
    if (flags & UDP_MULTI_DEFINED)
        return false;
    flags |= UDP_MULTI_DEFINED;

    // parsowanie

    udp_multicast = _udp_multicast;

    return true;
}

bool Radio_proxy::parse_udp_timeout(const std::string& _udp_timeout) {
    if (flags & UDP_TIMEOUT_DEFINED)
        return false;
    flags |= UDP_TIMEOUT_DEFINED;

    for (char c : _udp_timeout) {
        if (!std::isdigit(c))
            return false;
    }

    udp_timeout = std::stoi(_udp_timeout);

    return true;
}


Radio_proxy::Radio_proxy() {
    /* default values */
    flags = 0;
    metadata = false;
    timeout = 5;
    udp_flags = false;
    udp_timeout = 5;
    udp_multicast = "";
}

bool Radio_proxy::init(int argc, char* argv[]) {
    if ((argc % 2) != 1) 
        return false;
        
    for (int i = 1; i + 1 < argc; i += 2) {
        std::string curr = argv[i], next = argv[i + 1];
        if (curr == "-h") {
            if (!parse_host(next))
                return false;
        }
        else if (curr == "-r") {
            if (!parse_resource(next))
                return false;
        } 
        else if (curr == "-p") {
            if(!parse_port(next))
                return false;
        } 
        else if (curr == "-m") {
            if (!parse_metadata(next))
                return false;
        }
        else if (curr == "-t") {
            if (!parse_timeout(next))
                return false;
        }
        else if (curr == "-P") {
            if (!parse_udp_port(next))
                return false;
        }
        else if (curr == "-B") {
            if (!parse_udp_multicast(next))
                return false;
        }
        else if (curr == "-T") {
            if (!parse_udp_timeout(next))
                return false;
        }
        else {
            std::cerr << "wrong flag\n";
            return false;
        }
    }

    if (!(flags & HOST_DEFINED)) {
        std::cerr << "define host\n";
        return false;
    }
    if (!(flags & RESOURCE_DEFINED)) {
        std::cerr << "define resource\n";
        return false;
    }
    if (!(flags & PORT_DEFINED)) {
        std::cerr << "define port\n";
        return false;
    }

    /* A+B */
    if ((flags & UDP_PORT_DEFINED) || (flags & UDP_MULTI_DEFINED) || (flags & UDP_TIMEOUT_DEFINED)) {
        udp_flags = true;
        if (!(flags & UDP_PORT_DEFINED)) {
            std::cerr << "define UDP port\n";
            return false;
        }
    }

    return true;
}

std::string Radio_proxy::create_get_request() {
    std::string ret;
    ret.append("GET " + resource + " HTTP/1.1\r\n");
    ret.append("Host: " + host + "\r\n");
    ret.append("Accept: */*\r\n");
    if (metadata) {
        ret.append("Icy-MetaData: 1\r\n");
    }
    else {
        ret.append("Icy-MetaData: 0\r\n");
    }
    ret.append("Connection: close\r\n");
    ret.append("User-agent: radio-proxy\r\n");
    ret.append("\r\n");
    return ret;
}

void Radio_proxy::read_header(Tcp_socket &tcp_socket) {
    std::string curr_line = tcp_socket.socket_getline();
    curr_line = curr_line.substr(0, curr_line.size() - 2);
    /* checking first line of response */
    if (curr_line != OK_response1 && curr_line != OK_response2 && curr_line != OK_response3) {
        syserr("response not 200 OK");
    }
    while (curr_line != "\r\n") {
        curr_line = tcp_socket.socket_getline();
        size_t first = 0, last = 0, lastlast = curr_line.size()-1;
        for (size_t i = 0; i < curr_line.size(); i++) {
            if (curr_line[i] == ':') {
                first = i;
                last = i + 1;
                while (last < curr_line.size() && curr_line[last] == ' ') {
                    last++;
                }
                break;
            }
        }
        while (curr_line[lastlast] == '\r' || curr_line[lastlast] == '\n' || curr_line[lastlast] == ' ') {
            lastlast--;
        }
        std::string key = curr_line.substr(0, first);
        std::string value = curr_line.substr(last, lastlast-last+1);
        for(char &c : key) {
            c = (char)tolower(c);
        }
        header_info.insert(std::make_pair(key, value));
    }
    icy_metaint = -1;
    if (header_info.find("icy-metaint") != header_info.end()) { /* icy-metadata exists */
        icy_metaint = std::stoi(header_info[ICY_METAINT]);
        if (icy_metaint == 0) {
            icy_metaint = -1;
        }
    }
    if(icy_metaint == -1 && metadata) {
        syserr("no metatada sent by server, but metadata flag is set to true");
    }
}

std::string Radio_proxy::read_data(Tcp_socket &tcp_socket) {
    return tcp_socket.socket_read_n_bytes(icy_metaint);
}

std::string Radio_proxy::read_metadata(Tcp_socket &tcp_socket) {
    int bytes = 16 * (tcp_socket.socket_read_n_bytes(1)[0]);
    std::string data = tcp_socket.socket_read_n_bytes(bytes);
    size_t first = data.size() - 1;
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i] == ';') {
            first = i;
        }
    }
    data = data.substr(0, first+1);
    return data;
}

std::string Radio_proxy::read_continuous_data(Tcp_socket &tcp_socket, size_t buffer) {
    return tcp_socket.socket_read_n_bytes(buffer);
}

/* task A (main loop)*/
void Radio_proxy::no_udp_casting(Tcp_socket &tcp_socket) {

    std::string data_bytes, metadata_bytes;
    while (errno >= 0) {
        if (icy_metaint != -1) { /* metadata sent by server */
            data_bytes = read_data(tcp_socket);
            metadata_bytes = read_metadata(tcp_socket);
            std::cout << data_bytes;
            std::cerr << metadata_bytes;
        }
        else {
            data_bytes = read_continuous_data(tcp_socket, CONTINUOUS_BUFFER);
            std::cout << data_bytes;
        }
    }
}

/* task A+B (main loop) */
void Radio_proxy::udp_casting(Tcp_socket &tcp_socket) {

    Udp_socket udp_socket(udp_port, udp_multicast, udp_timeout);
    udp_socket.socket_connect();

    struct pollfd group[2];

    /* initialise TCP poll group */
    group[TCP_POLL].fd = tcp_socket.get_socket();
    group[TCP_POLL].events = (POLLIN | POLLOUT | POLLERR);
    group[TCP_POLL].revents = 0;

    /* initialise UDP poll group */
    group[UDP_POLL].fd = udp_socket.get_socket();
    group[UDP_POLL].events = (POLLIN | POLLOUT | POLLERR);
    group[UDP_POLL].revents = 0;

    std::string data_bytes, metadata_bytes;

    /* vector of all available clients */
    std::vector<std::tuple<sockaddr, socklen_t, std::clock_t>> clients; /* {addres, length of address, timestamp, flag} */

    std::clock_t prev_tcp_time = std::clock();

    while (errno >= 0) {
        group[TCP_POLL].revents = 0;
        group[UDP_POLL].revents = 0;

        int ret = poll(group, 2, 1000 * std::max(timeout, udp_timeout));
        if (ret < 0)
            syserr("poll");
        if (ret == 0)
            syserr("poll timeout");

        /* receive messages or radio signal from sockets */
        bool received_audio = false;
        if (group[TCP_POLL].revents & POLLIN) {
            /* check time of last socket read */
            std::clock_t curr_time = std::clock();
            long double delta_time = (long double)(curr_time-prev_tcp_time)/ (long double)CLOCKS_PER_SEC;
            prev_tcp_time = curr_time;
            if (delta_time >= timeout) {
                syserr("tcp socket timeout");
            }

            if (icy_metaint != -1) { /* metadata sent by server */
                data_bytes = read_data(tcp_socket);
                metadata_bytes = read_metadata(tcp_socket);
            }
            else {
                data_bytes = read_continuous_data(tcp_socket, CONTINUOUS_BUFFER);
            }
            received_audio = true;
        }

        if (group[UDP_POLL].revents & POLLIN) { /* receiving signal from client */
            auto addr_pair = udp_socket.receive_message();
            std::string ip_addr = inet_ntoa(((struct sockaddr_in *) &addr_pair.first)->sin_addr);
            auto client_tuple = Udp_socket::read_datagram(udp_socket.get_buffer());
            uint16_t type = std::get<0>(client_tuple);
            uint16_t length = std::get<1>(client_tuple);

            std::cout << "received data: " << std::get<0>(client_tuple) << " " << std::get<1>(client_tuple) << " [" << std::get<2>(client_tuple) << "]\n";

            int id = -1; /* id of client with this IP in clients array */
            for (size_t i = 0; i < clients.size(); i++) {
                if (ip_addr == inet_ntoa(((struct sockaddr_in *) &std::get<0>(clients[i]))->sin_addr)) {
                    id = i;
                }
            }

            if (type == DISCOVER) {
                if (id == -1) { /* no such ip address saved, we need to add it to vector */
                    clients.push_back(std::make_tuple(addr_pair.first, addr_pair.second, std::clock()));
                }
                else { /* if ip address exists in vector, wee treat DISCOVER as KEEPALIVE */
                    type = KEEPALIVE;
                }
                /* respond with IAM message (first create radio description)*/
                std::string description;
                if (header_info.find(ICY_NAME) != header_info.end())
                    description.append(header_info[ICY_NAME]);

                char *message = Udp_socket::create_datagram(IAM, description.length(), description);
                auto data = Udp_socket::read_datagram(message);
                std::cout << "IAM sent data: " << std::get<0>(data) << " " << std::get<1>(data) << " [" << std::get<2>(data) << "] " << "\n";

                udp_socket.send_message_direct(message, addr_pair.first, addr_pair.second);
            }
            if (type == KEEPALIVE) {
                /* update the timestamp */
                if (id != -1) {
                    std::get<2>(clients[id]) = std::clock();
                }
            }
        }

        /* check for timed out clients */
        std::clock_t curr_time = std::clock(); // todo lekko zbugowane (to swapowanie)
        for (size_t i = 0; i < clients.size(); i++) {
            /* what if client timed out */
            if (udp_timeout < (long double)(curr_time - std::get<2>(clients[i])) / (long double)CLOCKS_PER_SEC) {
                /* we need to erase him */
                std::swap(clients[i], clients.back());
                clients.pop_back();
            }
        }

        /* send radio signal to clients (if received) */
        if ((group[UDP_POLL].revents & POLLOUT) && received_audio) {
            /* send AUDIO */
            for (size_t i = 0; i < data_bytes.length(); i += DATA_LENGTH) { /* if radio sends audio longer that 2^16 */
                std::string to_send;
                for (size_t j = i; j < std::min(data_bytes.length(), i + DATA_LENGTH); j++) {
                    to_send.push_back(data_bytes[j]);
                }
                char *message = Udp_socket::create_datagram(AUDIO, to_send.length(), to_send);
                for (auto &client : clients) {

                    auto data = Udp_socket::read_datagram(message);
                    std::cout << "sending data: " << std::get<0>(data) << " " << std::get<1>(data) << " [" << std::get<2>(data) << "] "  << "\n";

                    udp_socket.send_message_direct(message, std::get<0>(client), std::get<1>(client));
                }
            }
            /* send METADATA */
            if (icy_metaint != -1) { /* metadata sent by server */
                char *message = Udp_socket::create_datagram(METADATA, metadata_bytes.length(), metadata_bytes);
                for (auto& client : clients) {
                    udp_socket.send_message_direct(message, std::get<0>(client), std::get<1>(client));
                }
            }
        }
    }
}


void Radio_proxy::start()
{
    std::cout << "starting radio-proxy\n";

    Tcp_socket tcp_socket(host, port, timeout);
    tcp_socket.socket_connect();
    tcp_socket.socket_send_request(create_get_request());

    read_header(tcp_socket);

    std::cout << "radio-proxy started, listening...\n";

    if (!udp_flags) { /* A */
        no_udp_casting(tcp_socket);
    }
    else { /* A+B */
        udp_casting(tcp_socket);
    }
}

int main(int argc, char* argv[]) {
    Radio_proxy radio;

    if (!radio.init(argc, argv)) {
        syserr("Radio_proxy::init()");
    }
    else {
        radio.start();
    }
    
    return 0;
}
