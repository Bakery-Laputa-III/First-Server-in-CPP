#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;

class Acceptor
{
    public:
        Acceptor(EventLoop *theLoop);
        ~Acceptor();

        void acceptConnection();

        void setNewConnectionCallBack(std::function<void(Socket*)> theCallBack);
    
    private:
        EventLoop *loop;
        Socket *listenSocket;
        std::function<void(Socket *)> newConnectionCallBack;
        Channel *listenChannel;
};
