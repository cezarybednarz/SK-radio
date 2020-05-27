#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>

#ifndef RADIO_PROXY_H
#define RADIO_PROXY_H

class Radio_proxy {
    
    unsigned flags;
    
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
    bool parse_host(const std::string& host);
    bool parse_resource(const std::string& resource);
    bool parse_port(const std::string& port);
    bool parse_metadata(const std::string& metadata);
    bool parse_timeout(const std::string& timeout);
};

#endif /* RADIO_PROXY_H */
