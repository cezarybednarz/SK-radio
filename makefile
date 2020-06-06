EXECS = radio-proxy
CXX = g++
CPPFLAGS = -Wall -Wextra -O2 -std=c++17
CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =
LDLIBS =

TARGET: $(EXECS)

all: radio_proxy.o tcp_socket.o udp_socket.o radio-proxy

err.o: err.h
tcp_socket: tcp_socket.h err.h
udp_socket: udp_socket.h err.h constants.h

radio_proxy.o: radio_proxy.cc err.h constants.h tcp_socket.h udp_socket.h
	$(CXX) $(CPPFLAGS) -c radio_proxy.cc

radio-proxy: radio_proxy.o err.o tcp_socket.o udp_socket.o
	$(CXX) $(CPPFLAGS) -o radio-proxy radio_proxy.o err.o tcp_socket.o udp_socket.o


.PHONY: clean TARGET
clean:
	rm -f $(EXECS) *.o *~
