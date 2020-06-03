
#include "tcp_socket.h"

Tcp_socket::Tcp_socket(const std::string& addr, const std::string& port)
 : connection_addr(const_cast<char*>(addr.c_str())),connection_port(const_cast<char*>(port.c_str()))
{ }

void Tcp_socket::socket_connect() {
    struct addrinfo addr_hints, *addr_result;

    std::memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; /* IPv4 */
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;

    int err = getaddrinfo(connection_addr, connection_port, &addr_hints, &addr_result);
    if (err == EAI_SYSTEM) /* system error */
        syserr("getaddrinfo: %s", gai_strerror(err));
    else if (err != 0) // other error (host not found, etc.)
        fatal("getaddrinfo: %s", gai_strerror(err));

    /* initialize socket according to getaddrinfo results */
    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0)
        syserr("socket");

    /* connect socket to the server */
    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
        syserr("connect");

    freeaddrinfo(addr_result);

    fp = fdopen(sock, "r");
    if (!fp)
        syserr("fdopen");
}

void Tcp_socket::socket_send_request(std::string content) {
    if (write(sock, content.c_str(), content.size()) < 0) {
        syserr("write");
    }
}

std::string Tcp_socket::socket_getline() {
    char *line_buf = nullptr;
    size_t line_buf_size = 0;
    if (getline(&line_buf, &line_buf_size, fp) < 0) {
        syserr("getline");
    }
    std::string ret(line_buf);
    return ret;
}

std::string Tcp_socket::socket_read_n_bytes(size_t n) {
    std::string ret;
    for (size_t i = 0; i < n; i++) {
        ret.push_back(fgetc(fp));
    }
    return ret;
}

char Tcp_socket::read_char() {
    return fgetc(fp);
}
