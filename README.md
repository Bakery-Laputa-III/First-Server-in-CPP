## 前言
此为Bakery记录实现第一个Server项目的过程，每一步都有一个分支，且所有的更改新增内容都放在本README中

## day 00
此项目是在[30dayMakeCppServer](https://github.com/yuesong-feng/30dayMakeCppServer)的教程下进行的，完成README的初始化

## day 01
实现最简单的C/S
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <cstring>
#include <cstdio>

int main() {
    // 创建监听套接字
    int server_listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 配置server监听IP与HOST
    struct sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);

    // 绑定监听套接字
    bind(server_listen_fd, (sockaddr*)&server_address, sizeof(server_address));

    // 开启监听
    listen(server_listen_fd, SOMAXCONN);

    // 初始化客户端地址结构体
    struct sockaddr_in client_address;
    socklen_t len_client_address = sizeof(client_address);
    std::memset(&client_address, 0 , len_client_address);

    int client_comm_fd = accept(server_listen_fd, (sockaddr*)&client_address, &len_client_address); // 就这样设计的size非要地址

    // 输出Client信息: IP and Port
    printf("New Client fd: %d! IP: %s Port: %d\n", client_comm_fd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    
    return 0;
}
```
```cpp
#include <sys/socket.h>
#include <arpa/inet.h>  // 包含了netinet/in.h
#include <cstring>

int main() {
    // 创建客户端socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 配置服务器地址信息
    struct sockaddr_in server_address;
    std::memset(&server_address, 0 , sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);

    connect(client_fd, (sockaddr*)&server_address, sizeof(server_address));

    return 0;
}
```

## day02
增加异常抛出功能, 与C/S消息传输功能
```cpp
void errif(bool condition, const char* errmsg)
{
    if (condition)
    {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

```
```cpp
int server_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(server_listen_fd == -1, "socket create error");
```
```cpp
while (true)
{
    char buffer[1024];
    std::memset(&buffer, 0, sizeof(buffer));

    ssize_t read_bytes = read(client_comm_fd, buffer, sizeof(buffer));
    
    if (read_bytes > 0)
    {
        // 打印从服务端接受的字节
        printf("message from client fd %d: %s\n", client_comm_fd, buffer);
        // 将该字节写回服务端
        write(client_comm_fd, buffer, sizeof(buffer));

    } else if (read_bytes == 0) {
        printf("client fd %d disconnected\n", client_comm_fd);
        close(client_comm_fd);
        break;
    } else if (read_bytes == -1) {
        close(client_comm_fd);
        errif(true, "socket read error");
    }
}
```
```cpp
while (true)
{
    char buffer[1024];
    std::memset(&buffer, 0, sizeof(buffer));

    scanf("%s", buffer);

    ssize_t write_bytes = write(server_fd, buffer, sizeof(buffer));

    if (write_bytes == -1)
    {
        printf("socket already disconnected, cant write anymore!\n");
        break;
    }

    std:memset(&buffer, 0, sizeof(buffer));
    
    int read_bytes = read(server_fd, buffer, sizeof(buffer));
    if (read_bytes > 0)
    {
        printf("message from server: %s\n", buffer);
    } else if (read_bytes == 0) {
        printf("server socket disconnected!\n");
    } else if (read_bytes == -1) {
        close(server_fd);
        errif(true, "socket read error");
    }
}
```

## day03
增加epoll管理机制,实现服务器的并发性,利用非阻塞socket完成对多个客户端消息传递的功能
```cpp
int epollfd = epoll_create1(0);
errif(epollfd == -1, "epoll create error");
```
```cpp
struct epoll_event events[MAX_EVENTS], evt;
std::memset(&events, 0, sizeof(events));
std::memset(&evt, 0, sizeof(evt));
```
```cpp
evt.data.fd = server_listen_fd;
evt.events = EPOLLIN | EPOLLET;
setnonblocking(server_listen_fd);
epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen_fd, &evt);
```
```cpp
// 将监听socket加入epoll管理
evt.data.fd = server_listen_fd;
evt.events = EPOLLIN | EPOLLET;
setnonblocking(server_listen_fd);
epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen_fd, &evt);

while (true)
{
    // 等待epoll事件发生,-1表示永久阻塞,直到事件发生
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    errif(nfds == -1, "epoll wait error");

    // 遍历所有就绪的socket符
    for (int i = 0; i < nfds; i++)
    {
        // 是监听socket的事件则表示有新的连接
        if (events[i].data.fd == server_listen_fd) 
        {
            // 初始化客户端地址结构体
            struct sockaddr_in client_address;
            socklen_t len_client_address = sizeof(client_address);
            std::memset(&client_address, 0 , len_client_address);

            // 创建新的socket接受客户端的连接
            int client_comm_fd = accept(server_listen_fd, (sockaddr*)&client_address, &len_client_address); // 就这样设计的size非要地址
            errif(client_comm_fd == -1, "socket accept error");

            // 输出Client信息: IP and Port
            printf("New Client fd: %d! IP: %s Port: %d\n", client_comm_fd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // 将新的客户端socket加入epoll管理
            std::memset(&evt, 0, sizeof(evt));
            evt.data.fd = client_comm_fd;
            evt.events = EPOLLIN | EPOLLET;
            setnonblocking(client_comm_fd);
            epoll_ctl(epollfd, EPOLL_CTL_ADD, client_comm_fd, &evt);
        } 
        // 可读事件(也即是客户端的write而非connect)
        else if (events[i].events & EPOLLIN)
        {
            char buffer[READ_SIZE];
            while (true)
            {
                std::memset(&buffer, 0 ,sizeof(buffer));
                ssize_t read_bytes = read(events[i].data.fd, buffer, sizeof(buffer));

                // 正常读取
                if (read_bytes > 0)
                {
                    printf("message from client fd %d: %s\n", events[i].data.fd, buffer);
                    write(events[i].data.fd, buffer, sizeof(buffer));
                } 
                // 客户端正常中断,继续读取
                else if (read_bytes == -1 && errno == EINTR)
                {
                    printf("continue reading");
                    continue;
                }
                // 读取完毕
                else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                {
                    printf("finish reading once, errno: %d\n", errno);
                    break;
                }
                // EOF客户端断开连接
                else if (read_bytes == 0)
                {
                    printf("EOF, client %d disconnected\n", events[i].data.fd);
                    close(events[i].data.fd);
                    break;
                }   
            }
        }
        else 
        {
            printf("something unexpected happened\n");
        }
    }
}
```

## day04
封装address, socket, epoll, handle, util, 重构C/S
```cpp
#pragma once
#include <arpa/inet.h>

class InternetAddress
{
    public:
        InternetAddress();
        InternetAddress(const char* ip, uint16_t port);
        ~InternetAddress();
        
    public:
        struct sockaddr_in address;
        socklen_t length;
};
```
```cpp
#pragma once

#include "./internet_address.hpp"

class Socket
{
    public:
        Socket();
        Socket(int);
        ~Socket();

        void bind(InternetAddress* address);
        void listen();
        void setNonBlocking();

        int accept(InternetAddress* address);

        void connect(InternetAddress* address);

        void close();

        int getFd() const;

    private:
        int fd;
};
```
```cpp
#pragma once

#include <sys/epoll.h>
#include <vector>

class Epoll
{
    public:
        Epoll();
        ~Epoll();

        void addFd(int fd, uint32_t op);
        std::vector<epoll_event> poll(int timeout = -1);

    private:
        int fd;
        struct epoll_event *events;
};
```
```cpp
#pragma once

void handleReadEvent(int fd);
```
```cpp
#ifndef UTIL_HPP
#define UTIL_HPP

void errif(bool, const char*);

#endif
```
```cpp
#include "../include/socket.hpp"
#include "../include/internet_address.hpp"
#include "../include/epoll.hpp"
#include "../include/handle.hpp"
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

    // 将监听套接字设置为非阻塞模式,且加入epoll管理
    listenSocket->setNonBlocking();
    serverEpoll->addFd(listenSocket->getFd(), EPOLLIN | EPOLLET);

    // 事件处理
    while (true)
    {
        // 在epoll中poll出事件
        std::vector<epoll_event> epollEvents = serverEpoll->poll();
        int numFds = epollEvents.size();

        // 事件处理
        for (int i = 0; i < numFds; i++)
        {
            // 新连接事件
            if (epollEvents[i].data.fd == listenSocket->getFd())
            {
                // 无delete,内存会发生泄露!
                InternetAddress *clientAddress = new InternetAddress();
                Socket *clientSocket = new Socket(listenSocket->accept(clientAddress));

                // 打印clientsocket的ip与port
                std::cout << "new client fd " << clientSocket->getFd() <<"!" << " IP: " << inet_ntoa(clientAddress->address.sin_addr) << " Port: " << ntohs(clientAddress->address.sin_port) << std::endl;
                
                // 将clientsocket设置成非阻塞模式且加入epoll中
                clientSocket->setNonBlocking();
                serverEpoll->addFd(clientSocket->getFd(), EPOLLIN | EPOLLET);
            }
            // 可读事件
            else if (epollEvents[i].events & EPOLLIN)
            {
                // 处理读取事件
                handleReadEvent(epollEvents[i].data.fd);
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
```
```cpp
#include "../include/socket.hpp"
#include "../include/internet_address.hpp"
#include "../include/util.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main() 
{
    // 创建客户端socket
    Socket *serverSocket = new Socket();
    
    // 配置服务器地址信息
    InternetAddress *serverAddress = new InternetAddress(SERVER_IP, SERVER_PORT);

    // 连接服务器
    serverSocket->connect(serverAddress);

    std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    while (true)
    {
        char buffer[BUFFER_SIZE];
        std::memset(buffer, 0, sizeof(buffer));

        // 获取用户输入
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));

        // 发送数据
        ssize_t writeBytes = write(serverSocket->getFd(), buffer, sizeof(buffer));
        if (writeBytes == -1)
        {
            std::cerr << "Socket already disconnected, can't write anymore!" << std::endl;
            break;
        }

        // 接收服务器响应
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t readBytes = read(serverSocket->getFd(), buffer, sizeof(buffer));
        
        if (readBytes > 0)
        {
            std::cout << "Message from server: " << buffer << std::endl;
        }
        else if (readBytes == 0)
        {
            std::cout << "Server disconnected!" << std::endl;
            break;
        }
        else if (readBytes == -1)
        {
            serverSocket->close();
            errif(true, "socket read error");
        }
    }

    // 清理资源
    delete serverSocket;
    delete serverAddress;
    
    return 0;
}
```

## day05
将事件封装为channel,同时更新epoll, server等对事件的处理到channel的处理
```cpp
#pragma once

#include <cstdint>

class Epoll;

class Channel
{
    public:
        Channel(Epoll *theEpoll, int theFd);
        ~Channel();

        int getFd() const;
        uint32_t getEvents() const;
        uint32_t getRevents() const;
        bool getInEpoll() const;

        void setInEpoll();
        void enableReading();
        void setRevents(uint32_t theEvents);

    private:
        Epoll *epoll;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
};
```
