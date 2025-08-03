#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include "../include/util.hpp"

#define MAX_EVENTS 1024
#define READ_SIZE 1024

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() 
{
    // 创建监听套接字
    int server_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(server_listen_fd == -1, "socket create error");

    // 配置server监听IP与HOST
    struct sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);

    // 绑定监听套接字
    errif(bind(server_listen_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1, "socket bind error");

    // 开启监听
    errif(listen(server_listen_fd, SOMAXCONN) == -1, "socket listen error");

    // 创建epoll
    int epollfd = epoll_create1(0);
    errif(epollfd == -1, "epoll create error");

    // 创建epoll事件组(for就绪), 事件(for监听)
    struct epoll_event events[MAX_EVENTS], evt;
    std::memset(&events, 0, sizeof(events));
    std::memset(&evt, 0, sizeof(evt));

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

    // 关闭监听socket
    close(server_listen_fd);

    return 0;
}
