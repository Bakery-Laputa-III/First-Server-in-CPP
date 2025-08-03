#include "../include/epoll.hpp"
#include "../include/util.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

#define EVENT_SIZE 1024

Epoll::Epoll() : fd(-1), events(nullptr)
{
    fd = ::epoll_create1(0);
    errif(fd == -1, "epoll create error");
    events = new epoll_event[EVENT_SIZE];
    std::memset(events, 0, sizeof(events));
}

Epoll::~Epoll()
{
    if (fd != 0)
    {
        close(fd);
        fd = -1;
    }

    delete[] events;
}

void Ep

