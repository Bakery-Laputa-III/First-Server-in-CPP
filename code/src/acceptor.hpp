#pragma once

#include <functional>
class EventLoop;
class Socket;
class InternetAddress;
class Channel;

class Acceptor
{
    public:
        Acceptor(EventLoop *theLoop);
        ~Acceptor();

        void acceptConnection();

        std::function<void(Socket*)> newConnectionCallBack;

        void setNewConnectionCallBack(std::function<void(Socket*)> theCallBack);
    
    private:
        EventLoop *loop;
        Socket *listenSocket;
        InternetAddress *listenAddress;
        Channel *listenChannel;
};
