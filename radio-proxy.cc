
#include "radio-proxy.h"


bool Radio_proxy::parse_host(std::string& host) {
    
}

bool Radio_proxy::parse_resource(std::string& resource) {
    
}

bool Radio_proxy::parse_port(std::string& port) {
    
}

bool Radio_proxy::parse_metadata(std::string &metadata) {
    
}

bool Radio_proxy::parse_timeout(std::string &timeout) {
    
}

bool Radio_proxy::init(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string s = argv[i];
        switch (s) {
            case "-h":
                flags |= HOST_DEFINED;
                
                break;
            case "-r":
                flags |= RESOURCE_DEFINED;
                
                break;
            case "-p":
                flags |= PORT_DEFINED;
                
                break;
                
            case "-m":
                flags |= METADATA_DEFINED;
                
                break;
                
            case "-t":
                flags |= TIMEOUT_DEFINED;
                
                break;
        }
    }
}


int main(int argc, char* argv[]) {
    Radio_proxy radio;
    radio.init(argc, argv);
    radio.start();
}
