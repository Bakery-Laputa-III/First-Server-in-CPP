#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include "../include/util.hpp"

int main() {
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

    // 初始化客户端地址结构体
    struct sockaddr_in client_address;
    socklen_t len_client_address = sizeof(client_address);
    std::memset(&client_address, 0 , len_client_address);

    int client_comm_fd = accept(server_listen_fd, (sockaddr*)&client_address, &len_client_address); // 就这样设计的size非要地址
    errif(client_comm_fd == -1, "socket accept error");

    // 输出Client信息: IP and Port
    printf("New Client fd: %d! IP: %s Port: %d\n", client_comm_fd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
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

    // 关闭监听socket
    close(server_listen_fd);

    return 0;
}