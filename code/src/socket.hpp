#pragma once

class InternetAddress;

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

        void connect(InternetAddress* address);

        void close();

        int getFd() const;

    private:
        int fd;
};