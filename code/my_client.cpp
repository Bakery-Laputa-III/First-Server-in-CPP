#include "src/socket.hpp"
#include "src/internet_address.hpp"
#include "src/util.hpp"
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