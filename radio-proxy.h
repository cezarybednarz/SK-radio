#include <iostream>
#include <cstdio>
#include <string>

#define HOST_DEFINED     (1 << 0) /* -h host */
#define RESOURCE_DEFINED (1 << 1) /* -r resource */
#define PORT_DEFINED     (1 << 2) /* -p port */
#define METADATA_DEFINED (1 << 3) /* -m yes|no (optional) */
#define TIMEOUT_DEFINED  (1 << 4) /* -t timeout (optional) */

class Radio-proxy {
    
    int flags; 
    
    
public:
    Radio_proxy() {
        flags = 0;
    }
    
    void init(int argc, char* argv[]);
    
private:
    bool parse_host(std::string host);
    bool parse_resource(std::string resource);
    bool parse_port(std::string port);
    bool parse_metadata(std::string metadata);
    bool parse_timeout(std::string timeout);
}
