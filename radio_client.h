#ifndef RADIO_CLIENT_H
#define RADIO_CLIENT_H


#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include <atomic>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <poll.h>

#include "err.h"

class Radio_client {
public:
    Radio_client();
    void init();
    void start();
};

#endif //RADIO_CLIENT_H
