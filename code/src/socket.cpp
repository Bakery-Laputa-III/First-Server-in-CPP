#include "../include/socket.hpp"
#include "../include/util.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

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

void Socket::bind(InternetAddress* theAddress)
{
    errif(::bind(fd, (sockaddr*)&theAddress->address, theAddress->length) == -1, "socket bind error");
}

void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setNonBlocking()
{
    fcntl(fd,  F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InternetAddress* theAdress)
{
    int clientFd = ::accept(fd, (sockaddr*)&theAdress->address, &theAdress->length);
    errif(clientFd == -1, "socket accept error");
    return clientFd;
}

void Socket::connect(InternetAddress* theAddress)
{
    int ret = ::connect(fd, (sockaddr*)&(theAddress->address), theAddress->length);
    errif(ret == -1, "socket connect error");
}

void Socket::close()
{
    errif(fd == -1, "socket close error");
    ::close(fd);
    fd = -1;
}

int Socket::getFd() const
{
    return fd;
}
