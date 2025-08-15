#pragma once

#include <functional>
#include <string>

class EventLoop;
class Socket;
class Buffer;
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
        Buffer *readBuffer;
        std::string *inBuffer;
        Channel *channel;
        std::function<void(Socket*)> deleteConnectionCallBack;
};
