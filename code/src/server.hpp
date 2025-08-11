#pragma once

class EventLoop;
class Socket;
class Server
{
    public:
        Server(EventLoop *theLoop);
        ~Server();

        void newConnection(Socket *listen_socket);
        void handleReadEvent(int fd);
    private:
        EventLoop *loop;
};
