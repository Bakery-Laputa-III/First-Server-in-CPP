#pragma once
#include <arpa/inet.h>

class InternetAddress
{
    public:
        InternetAddress();
        InternetAddress(char* ip, uint16_t port);
        ~InternetAddress();
        
    public:
        struct sockaddr_in address;
        socklen_t length;
};