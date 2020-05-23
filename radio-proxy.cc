#include <iostream>
#include <cstdio>

#include "err.h"

#define HOST_DEFINED     (1 << 0) /* -h host */
#define RESOURCE_DEFINED (1 << 1) /* -r resource */
#define PORT_DEFINED     (1 << 2) /* -p port */
#define METADATA_DEFINED (1 << 3) /* -m yes|no (optional) */
#define TIMEOUT_DEFINED  (1 << 4) /* -t timeout (optional) */

int parse_host(std::string& host) {
    
}

int parse_resource(std::string& resource) {
    
}

int parse_port(std::string& port) {
    
}

int parse_metadata(std::string &metadata) {
    
}

int parse_timeout(std::string &timeout) {
    
}

int parse_parameters(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string s = argv[i];
        if (s == "-m") {
            
        }
        else if (s == "
    }
}

int main(int argc, char* argv[]) {
    if (parse_parameters(argc, argv) != 0) {
        syserr("bad params");
    }
}
