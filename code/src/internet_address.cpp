#include "internet_address.hpp" 
#include <arpa/inet.h> 
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

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

void InternetAddress::setInternetAddress(sockaddr_in theAddress, socklen_t theAddressLength)
{
    address = theAddress;
    length = theAddressLength;
}

sockaddr_in InternetAddress::getAddress() const { return address; }

socklen_t InternetAddress::getAddressLength() const { return length; }
