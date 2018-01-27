#include "Channel.h"
#include "Poller.h"
#include "EventLoop.h"

#include <assert.h>
using namespace Happy;
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
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_)
    {
        activeChannels.clear();
        poller_->poll(kPollTimeMs,&activeChannels_);  //利用poll来获取 activeChannels
        for(ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end() ; ++it)
        {
            (*it) -> handleEvent();
        }
    }
    cout <<" Loop Stop "<<endl;
    looping_ = false;
}


void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel * channel )
{
    assert(channel->ownerLoop == this);
    assertInLoopThread();
    poller_ -> updateChannel(channel);
}

//void EventLoop::


















