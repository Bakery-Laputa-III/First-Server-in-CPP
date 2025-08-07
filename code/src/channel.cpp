#include "../include/channel.hpp"
#include "../include/epoll.hpp"

Channel::Channel(Epoll *theEpoll, int theFd) : epoll(theEpoll), fd(theFd) {}

Channel::~Channel() {}

int Channel::getFd() const { return fd; }

uint32_t Channel::getEvents() const { return events; }

uint32_t Channel::getRevents() const { return revents; }

bool Channel::getInEpoll() const { return inEpoll; }

void Channel::setInEpoll() { inEpoll = true; }

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    epoll->updateChannel(this);
}

void Channel::setRevents(uint32_t theEvents) { revents = theEvents; }
