#pragma once

#include <cstdint>

class Epoll;

class Channel
{
    public:
        Channel(Epoll *theEpoll, int theFd);
        ~Channel();

        int getFd() const;
        uint32_t getEvents() const;
        uint32_t getRevents() const;
        bool getInEpoll() const;

        void setInEpoll();
        void enableReading();
        void setRevents(uint32_t theEvents);

    private:
        Epoll *epoll;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
};