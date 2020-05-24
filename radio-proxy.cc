
#include "radio-proxy.h"


bool Radio_proxy::parse_host(const std::string host) {
    if (flags & HOST_DEFINED) 
        return false;
    flags |= HOST_DEFINED;
    
    return true;
}

bool Radio_proxy::parse_resource(const std::string resource) {
    if (flags & RESOURCE_DEFINED) 
        return false;
    flags |= RESOURCE_DEFINED;
    
    return true;
}

bool Radio_proxy::parse_port(const std::string port) {
    if (flags & PORT_DEFINED)
        return false;
    flags |= PORT_DEFINED;
    
    return true;
}

bool Radio_proxy::parse_metadata(const std::string metadata) {
    if (flags & METADATA_DEFINED)
        return false;
    flags |= METADATA_DEFINED;
    
    return true;
}

bool Radio_proxy::parse_timeout(const std::string timeout) {
    if (flags & TIMEOUT_DEFINED)
        return false;
    flags |= TIMEOUT_DEFINED;
    
    return true;
}

Radio_proxy::Radio_proxy() {
    flags = 0;
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
    return true;
}

void Radio_proxy::start() {
    std::cout << "radio-proxy started\n";
}


int main(int argc, char* argv[]) {
    Radio_proxy radio;
    
    if (!radio.init(argc, argv)) {
        printf("error while starting\n");
    }
    else {
        radio.start();
    }
    
    return 0;
}
