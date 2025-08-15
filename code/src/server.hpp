#pragma once

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server
{
    public:
        Server(EventLoop *theLoop);
        ~Server();

        void newConnection(Socket *listen_socket);
        void deleteConnection(Socket *socket);

    private:
        EventLoop *loop;
        Acceptor *acceptor;
        std::map<int, Connection *> connections;
};
