#include "Poller.h"
#include "Channel.h"

#include "logging/Logging.h"
#include <assert.h>
#include <poll.h>
using namespace Happy;

Poller::Poller(EventLoop* loop)
 : ownerLoop_(loop)
 {
 }

Poller::~Poller()
{
}

Timestap Poller::poll(int timeoutMs, ChannelList * activeChannels)
{
    int numEvents = ::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
    Timestap now(Timestap::now());
    if(numEvents>0)
    {
        cout << numEvents<<" events happended";
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents == 0 )
    {
        cout << "Nothing happended"<<endl;
    }
    else 
    {
        cout <<"Poller::poll()"<<endl;
    }
    return now;
}

