#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll
{
    public:
        Epoll();
        ~Epoll();

        void addFd(int fd, uint32_t op);
        void updateChannel(Channel *theChannel);
        std::vector<Channel*> poll(int timeout = -1);

    private:
        int fd;
        struct epoll_event *events;
};