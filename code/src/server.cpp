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