# 简洁版 Makefile
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Iinclude

all: server client

server: src/server.cpp src/socket.cpp src/internet_address.cpp src/epoll.cpp src/handle.cpp src/util.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

client: src/client.cpp src/socket.cpp src/internet_address.cpp src/util.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f server client