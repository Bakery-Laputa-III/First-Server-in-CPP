#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;

class Connection
{
    public:
        Connection(EventLoop *theLoop, Socket *theSocket);
        ~Connection();

        void echo(int socket);
        void setDeleteConnectionCallBack(std::function<void(Socket*)>);

    private:
        EventLoop *loop;
        Socket *socket;
        Channel *channel;
        std::function<void(Socket*)> deleteConnectionCallBack;
};
