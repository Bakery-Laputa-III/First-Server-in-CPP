#include <sys/socket.h>
#include <arpa/inet.h>  // 包含了netinet/in.h
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include "../include/util.hpp"

#define BUFFER_SIZE 1024

int main() 
{
    // 创建客户端socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(server_fd == -1, "socket create error");

    // 配置服务器地址信息
    struct sockaddr_in server_address;
    std::memset(&server_address, 0 , sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);

    errif(connect(server_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1, "socket connect error");

    while (true)
    {
        char buffer[BUFFER_SIZE];
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

    close(server_fd);
    
    return 0;
}
