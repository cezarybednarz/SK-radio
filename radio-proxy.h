#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>

#ifndef RADIO_PROXY_H
#define RADIO_PROXY_H

#define HOST_DEFINED     (1 << 0) /* -h host */
#define RESOURCE_DEFINED (1 << 1) /* -r resource */
#define PORT_DEFINED     (1 << 2) /* -p port */
#define METADATA_DEFINED (1 << 3) /* -m yes|no (optional) */
#define TIMEOUT_DEFINED  (1 << 4) /* -t timeout (optional) */

class Radio_proxy {
    
    int flags; 
    
    std::string host;
    std::string resource;
    int port;
    bool metadata;
    int timeout;
    
    
public:
    Radio_proxy();
    
    bool init(int argc, char* argv[]);
    
    void start(); 
    
private:
    bool parse_host(const std::string host);
    bool parse_resource(const std::string resource);
    bool parse_port(const std::string port);
    bool parse_metadata(const std::string metadata);
    bool parse_timeout(const std::string timeout);
};

#endif /* RADIO_PROXY_H */
