#include "Channel.h"
#include "Poller.h"
#include "Logging.h"


#include <assert.h>
using namespace muduo;
__thread EventLoop* t_loopInThisThread =0 ;
const int kPollTimeMs = 10000;
EventLoop::EventLoop()
    :looping_(false),
    quit_(false),
    threadId_(CurrentThread::tid());
    poller_(new Poller(this))

{
    //FIXME should use the log 
    cout <<"EventLoop create " << this << "in thread "<<threadId_;
    if(t_loopInThisThread)
    {
        
    }
    else 
    {
        t_loopInThisThread = this;
    }
}


EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{

}







