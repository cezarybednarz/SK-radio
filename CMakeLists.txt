cmake_minimum_required(VERSION 3.0)
project(zadanie_2 CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

# Czy chcemy oglądać komunikatów o procentowym postępie kompilacji?
set_property(GLOBAL PROPERTY RULE_udp_msgS ON)

# Czy chcemy zobaczyć polecenia wywołane przez make?
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wextra -O2 -std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -std=c++17 -g -DDEBUG")

# tutaj wpisywać
set(SOURCE_FILES_PROXY radio_proxy.cc constants.h tcp_socket.cc tcp_socket.h err.h err.cc udp_sender.cc udp_sender.h)
set(SOURCE_FILES_CLIENT radio_client.cc radio_client.h)


add_executable(radio-proxy ${SOURCE_FILES_PROXY})
add_executable(radio-client ${SOURCE_FILES_CLIENT})
