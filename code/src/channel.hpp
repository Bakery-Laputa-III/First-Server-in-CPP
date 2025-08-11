#pragma once

#include <cstdint>
#include <functional>

class Epoll;
class EventLoop;

class Channel
{
    public:
        Channel(EventLoop *theLoop, int theFd);
        ~Channel();

        int getFd() const;
        uint32_t getEvents() const;
        uint32_t getRevents() const;
        bool getInEpoll() const;

        void setInEpoll();
        void handleEvent();
        void enableReading();

        void setRevents(uint32_t theEvents);
        void setCallBack(std::function<void()> theCallBack);
    private:
        EventLoop *loop;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
        std::function<void()> callback;
};