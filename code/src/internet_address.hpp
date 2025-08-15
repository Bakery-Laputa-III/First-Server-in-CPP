#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class InternetAddress
{
    public:
        InternetAddress();
        InternetAddress(const char* ip, uint16_t port);
        ~InternetAddress();

        void setInternetAddress(sockaddr_in theAddress, socklen_t theAddressLength);
        sockaddr_in getAddress() const;
        socklen_t getAddressLength() const;
        
    private:
        struct sockaddr_in address;
        socklen_t length;
};