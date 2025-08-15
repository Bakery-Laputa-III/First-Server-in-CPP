# 完整的 Makefile
CXX := clang++
CXXFLAGS := -std=c++11 -Wall -g -Isrc

all: my_server my_client

my_server: my_server.cpp src/connection.cpp src/acceptor.cpp src/server.cpp src/socket.cpp src/internet_address.cpp src/epoll.cpp src/handle.cpp src/util.cpp src/channel.cpp src/event_loop.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

my_client: my_client.cpp src/socket.cpp src/internet_address.cpp src/util.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f my_server my_client *.o