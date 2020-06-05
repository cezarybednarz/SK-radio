#include "radio_client.h"
#include "udp_socket.h"

Radio_client::Radio_client() {

}

bool Radio_client::init(int argc, char *argv[]) {
    return true;
}

void Radio_client::start() {
    std::cout << "Starting radio-client\n";

    Udp_socket udp_socket;




    std::cout << "radio-client started...\n";

    while(errno >= 0) {

    }
}

void send_discover() {

}

void send_keepalive() {

}

int main(int argc, char* argv[]) {
    Radio_client radio_client;
    if (!radio_client.init(argc, argv)) {
        syserr("Radio_client::init()")
    }
    else {
        radio_client.start();
    }
    return 0;
}