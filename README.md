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
