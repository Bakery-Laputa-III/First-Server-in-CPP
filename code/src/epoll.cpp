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
    std::memset(events, 0, sizeof(events) * EVENT_SIZE);
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

void Epoll::addFd(int theFd, uint32_t op)
{
    struct epoll_event evt;
    std::memset(&evt, 0, sizeof(evt));

    evt.data.fd = theFd;
    evt.events = op;

    // 事件加入epoll中
    errif(epoll_ctl(fd, EPOLL_CTL_ADD, theFd, &evt) == -1, "epoll add error");
}

std::vector<epoll_event> Epoll::poll(int timeout)
{
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(fd, events, EVENT_SIZE, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; i++) activeEvents.push_back(events[i]);
    return activeEvents;
}

