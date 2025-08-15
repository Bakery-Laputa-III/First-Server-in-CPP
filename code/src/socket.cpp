#include "socket.hpp"
#include "internet_address.hpp"
#include "util.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

Socket::Socket() : fd(-1)
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int theFd) : fd(theFd)
{
    errif(fd == -1, "socket create error");
}

Socket::~Socket()
{
    if (fd != -1)
    {
        this->close();
    }
}

void Socket::bind(InternetAddress *theAddress)
{
    struct sockaddr_in address = theAddress->getAddress();
    socklen_t length = theAddress->getAddressLength();

    errif(::bind(fd, (sockaddr*)&address, length) == -1, "socket bind error");
}

void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setNonBlocking()
{
    fcntl(fd,  F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InternetAddress *theAdress)
{
    sockaddr_in address;
    socklen_t length = sizeof(address);
    std::memset(&address, 0, sizeof(address));

    int clientFd = ::accept(fd, (sockaddr*)&address, &length);
    errif(clientFd == -1, "socket accept error");

    theAdress->setInternetAddress(address, length);    
    return clientFd;
}

int Socket::getFd() const
{
    return fd;
}

void Socket::close()
{
    errif(fd == -1, "socket close error");
    ::close(fd);
    fd = -1;
}
