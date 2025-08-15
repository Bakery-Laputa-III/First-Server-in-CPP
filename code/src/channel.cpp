#include "channel.hpp"
#include "event_loop.hpp"
#include <sys/epoll.h>
#include <functional>
#include <unistd.h>

Channel::Channel(EventLoop *theLoop, int theFd) : loop(theLoop), fd(theFd) {}

Channel::~Channel() 
{
    if(fd != -1)
    {
        close(fd);
    }
}

int Channel::getFd() const { return fd; }

uint32_t Channel::getEvents() const { return events; }

uint32_t Channel::getRevents() const { return revents; }

bool Channel::getInEpoll() const { return inEpoll; }

void Channel::setInEpoll() { inEpoll = true; }

void Channel::handleEvent()
{
    callback();
}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

void Channel::setRevents(uint32_t theEvents) { revents = theEvents; }

void Channel::setCallBack(std::function<void()> theCallBack) { callback = theCallBack; }
