#pragma once

#include "./internet_address.hpp"

class Socket
{
    public:
        Socket();
        Socket(int);
        ~Socket();

        void bind(InternetAddress* address);
        void listen();
        void setNonBlocking();

        int accept(InternetAddress* address);

        int getFd() const;

    private:
        int fd;
};