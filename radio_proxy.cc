
#include "radio_proxy.h"


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

    // parsowanie

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

Radio_proxy::Radio_proxy() {
    flags = 0;
    
    /* default values */
    metadata = false;
    timeout = 5;
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
        else {
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

    return true;
}

std::string Radio_proxy::create_get_request() {
    std::string ret;
    ret.append("GET " + resource + " HTTP/1.1\r\n");
    ret.append("Host: " + host + "\r\n");
    ret.append("Accept: */*\r\n");
    ret.append("Icy-MetaData: 1\r\n");
    ret.append("Connection: close\r\n");
    ret.append("User-agent: radio-proxy\r\n");
    ret.append("\r\n");
    return ret;
}

void Radio_proxy::start() {
    std::cout << "starting radio-proxy\n";

    Tcp_socket tcp_socket(host, port);
    tcp_socket.socket_connect();
    tcp_socket.socket_send_request(create_get_request());

    std::cout << create_get_request() << "\n";

    std::string curr_line = tcp_socket.socket_getline();
    while (curr_line != "\r\n") {
        curr_line = tcp_socket.socket_getline();
        std::cout << curr_line << "\n";
    }

    std::cout << "radio-proxy started\n";
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
