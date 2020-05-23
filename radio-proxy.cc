
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
        switch (argv[i]) {
            case "-h":
                if (!parse_host(argv[i + 1])
                    return false;
                break;
                
            case "-r":
                if (!parse_resource(argv[i + 1]))
                    return false;
                break;
                
            case "-p":
                if(!parse_port(argv[i + 1]))
                    return false;
                break;
                
            case "-m":
                if (!parse_metadata(argv[i + 1])
                    return false;
                break;
                
            case "-t":
                if (!parse_timeout(argv[i + 1])
                    return false;
                break;
                
            default:
                return false;
                break;
        }
    }
    return true;
}

void Radio_proxy::start() {
    std::cout << "radio-proxy started\n";
}


int main(int argc, char* argv[]) {
    Radio_proxy radio;
    radio.init(argc, argv);
    radio.start();
    
    return 0;
}
