#include "epoll.hpp"
#include <vector>
#include "event_loop.hpp"
#include "channel.hpp"

EventLoop::EventLoop() : epoll(nullptr), quit(false)
{
    epoll = new Epoll();
}

EventLoop::~EventLoop()
{
    delete epoll;
}

void EventLoop::loop()
{
    while(quit == false)
    {
        std::vector<Channel*> channels = epoll->poll();
        for (auto it = channels.begin(); it != channels.end(); ++it) (*it)->handleEvent();
    }
}

void EventLoop::updateChannel(Channel *theChannel)
{
    epoll->updateChannel(theChannel);
}
