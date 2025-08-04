#include "../include/internet_address.hpp" 
#include "../include/util.hpp"
#include <arpa/inet.h> 
#include <string>
#include <cstring>

InternetAddress::InternetAddress() : length(sizeof(address))
{
    std::memset(&address, 0 ,sizeof(address));
}

InternetAddress::InternetAddress(const char* ip, uint16_t port) : length(sizeof(address))
{
    std::memset(&address, 0 ,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
}

InternetAddress::~InternetAddress() {}
