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