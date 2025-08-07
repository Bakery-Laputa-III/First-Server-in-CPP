#include "../include/socket.hpp"
#include "../include/internet_address.hpp"
#include "../include/epoll.hpp"
#include "../include/handle.hpp"
#include "../include/channel.hpp"
#include <vector>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <iostream>

#define SERVER_LISTEN_IP "127.0.0.1"
#define SERVER_LISTEN_PORT 8888

int main()
{
    // 创建监听套接字
    Socket *listenSocket = new Socket();
    // 配置服务器监听地址端口
    InternetAddress *listenAdress = new InternetAddress(SERVER_LISTEN_IP, SERVER_LISTEN_PORT);
    // 用监听套接字监听该地址端口
    listenSocket->bind(listenAdress);
    // 开启监听
    listenSocket->listen();

    // 创建epoll管理
    Epoll *serverEpoll = new Epoll();

    // 将监听套接字设置为非阻塞模式,创建监听channel(设置为RAED)
    listenSocket->setNonBlocking();
    Channel* listenChannel = new Channel(serverEpoll, listenSocket->getFd());
    listenChannel->enableReading();

    // 事件处理
    while (true)
    {
        // 在epoll中poll出事件
        std::vector<Channel*> activeChannels = serverEpoll->poll();
        int numFds = activeChannels.size();

        // 事件处理
        for (int i = 0; i < numFds; i++)
        {
            int channelFd = activeChannels[i]->getFd();
            // 新连接channel
            if (channelFd == listenSocket->getFd())
            {
                // 无delete,内存会发生泄露!
                InternetAddress *clientAddress = new InternetAddress();
                Socket *clientSocket = new Socket(listenSocket->accept(clientAddress));

                // 打印clientsocket的ip与port
                std::cout << "new client fd " << clientSocket->getFd() <<"!" << " IP: " << inet_ntoa(clientAddress->address.sin_addr) << " Port: " << ntohs(clientAddress->address.sin_port) << std::endl;
                
                // 将clientsocket设置成非阻塞模式, 创建clientChannel
                clientSocket->setNonBlocking();
                Channel *clientChannel = new Channel(serverEpoll,  clientSocket->getFd());
                clientChannel->enableReading();
            }
            // 可读事件
            else if (activeChannels[i]->getRevents() & EPOLLIN)
            {
                // 处理读取事件
                handleReadEvent(channelFd);
            }
            // 其他事件(未开发)
            else 
            {
                std::cout << "something unexpected happened!\n";
            }
        }
    }   

    // 删除堆建变量
    delete listenSocket;
    delete listenAdress;
    delete serverEpoll;

    return 0;
}