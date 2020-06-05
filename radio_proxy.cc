
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

void Radio_proxy::start()
{
    std::cout << "starting radio-proxy\n";

    Tcp_socket tcp_socket(host, port, timeout);
    tcp_socket.socket_connect();
    tcp_socket.socket_send_request(create_get_request());

    Udp_socket udp_socket(udp_port, udp_multicast, udp_timeout);
    udp_socket.socket_connect();

    read_header(tcp_socket);

    std::cout << "radio-proxy started, listening...\n";

    std::string data_bytes, metadata_bytes;

    if (!udp_flags) { /* A */
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
    else { /* A+B */
        while (errno >= 0) {
            auto addr_pair = udp_socket.receive_message();
            udp_socket.send_message_direct("XDD", addr_pair.first, addr_pair.second);
        }
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
