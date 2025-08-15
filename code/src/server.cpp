#include "acceptor.hpp"
#include "socket.hpp"
#include "server.hpp"
#include "event_loop.hpp"
#include "channel.hpp"
#include "acceptor.hpp"
#include "connection.hpp"

#include <functional>
#include <cstring>
#include <unistd.h>

Server::Server(EventLoop *theLoop) : loop(theLoop), acceptor(nullptr)
{
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> callBack = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallBack(callBack);
}
 

Server::~Server() 
{ 
    delete acceptor;
}

void Server::newConnection(Socket *socket)
{
    Connection *connection = new Connection(loop, socket);
    std::function<void(Socket *)> callBack = std::bind(&Server::deleteConnection, this, std::placeholders:: _1);
    connection->setDeleteConnectionCallBack(callBack);
    connections[socket->getFd()] = connection;
}

void Server::deleteConnection(Socket *socket)
{
    Connection *connection = connections[socket->getFd()];
    connections.erase(socket->getFd());
    delete connection;
}
