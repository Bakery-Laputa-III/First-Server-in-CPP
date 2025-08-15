#pragma once

class InternetAddress;

class Socket
{
    public:
        Socket();
        Socket(int theFd);
        ~Socket();

        void bind(InternetAddress* address);
        void listen();
        void setNonBlocking();

        int accept(InternetAddress* address);

        int getFd() const;

        void close();

    private:
        int fd;
};