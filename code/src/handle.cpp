#include <cstring>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#define READ_SIZE 1024

void handleReadEvent(int fd)
{
    char buffer[READ_SIZE];

    // fd为非阻塞IO
    while (true)
    {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t readBytes = read(fd, buffer, sizeof(buffer));
        // 读到内容
        if (readBytes > 0)
        {
            std::cout << "message from client fd " << fd << ": " << buffer << std::endl;
            write(fd, buffer, sizeof(buffer));
        }
        // 客户正常终端,继续读取
        else if (readBytes == -1 && errno == EINTR)
        {
            std::cout << "continue reading";
            continue;
        }
        // 读取完毕
        else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        }
        // EOF,客户端断开连接
        else if (readBytes == 0)
        {
            std::cout << "EOF, client fd " << fd << " disconnected!" << std::endl;
            close(fd);
            break;
        }
    }
}