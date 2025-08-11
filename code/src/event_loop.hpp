#pragma once

class Epoll;
class Channel;

class EventLoop
{
    public:
        EventLoop();
        ~EventLoop();

        void loop();
        void updateChannel(Channel *theChannel);
    private:
        Epoll *epoll;
        bool quit;
};
