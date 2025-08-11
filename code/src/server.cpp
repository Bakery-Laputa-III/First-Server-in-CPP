#include "socket.hpp"
#include "internet_address.hpp"
#include "server.hpp"
#include "event_loop.hpp"
#include "channel.hpp"

#include <functional>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define SERVER_LISTEN_IP "127.0.0.1"
#define SERVER_LISTEN_PORT 8888
#define READ_BUFFER 1024

Server::Server(EventLoop *theLoop) : loop(theLoop)
{
    // 创建监听套接字
    Socket *listenSocket = new Socket();
    // 配置服务器监听地址端口
    InternetAddress *listenAdress = new InternetAddress(SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    // 用监听套接字监听该地址端口
    listenSocket->bind(listenAdress);
    // 开启监听
    listenSocket->listen();
    // 将监听套接字设置为非阻塞模式
    listenSocket->setNonBlocking();

    Channel* listenChannel = new Channel(loop, listenSocket->getFd());
    std::function<void()> callBack = std::bind(&Server::newConnection, this, listenSocket);
    
    listenChannel->setCallBack(callBack);
    listenChannel->enableReading();
} 

Server::~Server() { }

void Server::handleReadEvent(int fd)
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
            break;
        }
    }
}

void Server::newConnection(Socket *listenSocket)
{
    // 无delete,内存会发生泄露!
    InternetAddress *clientAddress = new InternetAddress();
    Socket *clientSocket = new Socket(listenSocket->accept(clientAddress));

    // 打印clientsocket的ip与port
    std::cout << "new client fd " << clientSocket->getFd() <<"!" << " IP: " << inet_ntoa(clientAddress->address.sin_addr) << " Port: " << ntohs(clientAddress->address.sin_port) << std::endl;
    
    // 将clientsocket设置成非阻塞模式, 创建clientChannel
    clientSocket->setNonBlocking();
    Channel *clientChannel = new Channel(loop,  clientSocket->getFd());
    std::function<void()> callBack = std::bind(&Server::handleReadEvent, this, clientSocket->getFd());
    clientChannel->setCallBack(callBack);
    clientChannel->enableReading();
}