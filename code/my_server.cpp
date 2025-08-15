#include "src/server.hpp"
#include "src/event_loop.hpp"

int main()
{
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    
    loop->loop();

    delete server;
    delete loop;

    return 0;
}
