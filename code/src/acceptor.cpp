#include "acceptor.hpp"
#include "event_loop.hpp"
#include "socket.hpp"
#include "internet_address.hpp"
#include "channel.hpp"
#include <functional>

#define SERVER_LISTEN_IP "127.0.0.1"
#define SERVER_LISTEN_PORT 8888

Acceptor::Acceptor(EventLoop *theLoop) : loop(theLoop) 
{
    // 创建监听套接字
    Socket *listenSocket = new Socket();
    // 配置服务器监听地址端口
    InternetAddress *listenAddress = new InternetAddress(SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    // 用监听套接字监听该地址端口
    listenSocket->bind(listenAddress);
    // 开启监听
    listenSocket->listen();
    // 将监听套接字设置为非阻塞模式
    listenSocket->setNonBlocking();

    Channel* listenChannel = new Channel(loop, listenSocket->getFd());
    std::function<void()> callBack = std::bind(&Acceptor::acceptConnection, this);
    
    listenChannel->setCallBack(callBack);
    listenChannel->enableReading();
}

Acceptor::~Acceptor()
{
    delete listenSocket;
    delete listenAddress;
    delete listenChannel;
}

void Acceptor::acceptConnection()
{
    newConnectionCallBack(listenSocket);
}

void Acceptor::setNewConnectionCallBack(std::function<void(Socket*)> theCallBack)
{
    newConnectionCallBack = theCallBack;
}
