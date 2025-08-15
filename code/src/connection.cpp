#include "connection.hpp"
#include "channel.hpp"
#include "event_loop.hpp"
#include "socket.hpp"
#include <functional>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *theLoop, Socket *theSocket) : loop(theLoop), socket(theSocket), channel(nullptr)
{
    channel = new Channel(loop, socket->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallBack(cb);
    channel->enableReading();
}

Connection::~Connection()
{
    delete channel;
    delete socket;
}

void Connection::echo(int fd)
{
    char buffer[READ_BUFFER];

    // socketFd为非阻塞IO
    while (true)
    {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t readBytes = read(fd, buffer, sizeof(buffer));
        // 读到内容
        if (readBytes > 0)
        {
            std::cout << "message from client fd " << fd << ": " << buffer << std::endl;
            write(fd, buffer, readBytes);
        }
        // 客户正常终端,继续读取
        else if (readBytes == -1 && errno == EINTR)
        {
            std::cout << "continue reading";
            continue;
        }
        // 读取完毕
        else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        }
        // EOF,客户端断开连接
        else if (readBytes == 0)
        {
            std::cout << "EOF, client fd " << fd << " disconnected!" << std::endl;
            close(fd);
            deleteConnectionCallBack(socket);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallBack(std::function<void(Socket*)> theCallBack)
{
    deleteConnectionCallBack = theCallBack;
}
