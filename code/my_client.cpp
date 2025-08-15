#include "src/buffer.hpp"
#include "src/socket.hpp"
#include "src/internet_address.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() 
{
    // 创建套接字
    Socket *sock = new Socket();

    // 创建服务器地址
    InternetAddress *server_addr = new InternetAddress(SERVER_IP, SERVER_PORT);

    // 连接
    sock->connect(server_addr);

    int sockfd = sock->getFd();

    Buffer *send_buf = new Buffer();
    Buffer *read_buf = new Buffer();

    while (true)
    {
        send_buf->getline();

        ssize_t write_bytes = write(sockfd, send_buf->c_str(), send_buf->size());

        if (write_bytes == -1)
        {
            std::cout << "socket already disconnected, can't write anymore!\n";
            break;
        }

        int already_read = 0;
        char buffer[1024];

        while (true)
        {
            std::memset(buffer, 0 ,sizeof(buffer));
            ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));

            if (read_bytes > 0)
            {
                read_buf->append(buffer, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0)
            {
                std::cout << "server disconnected!\n";
                exit(EXIT_SUCCESS);
            }

            if (already_read >= send_buf->size())
            {
                std::cout << "message from server: " << read_buf->c_str() << std::endl;
                break;
            }
        }

        read_buf->clear();
    }

    delete sock;
    delete server_addr;
    delete read_buf;
    delete  send_buf;

    return 0;
}