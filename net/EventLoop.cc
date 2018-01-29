#include "Channel.h"
#include "Poller.h"
#include "EventLoop.h"

#include <iostream>
#include <assert.h>
using namespace Happy;

__thread EventLoop* t_loopInThisThread =0 ;

const int kPollTimeMs = 1000; //poll等待秒数
EventLoop::EventLoop()
    :looping_(false),
    quit_(false),
    threadId_(0), //FIXME use the thread
    poller_(new Poller(this))

{
    //FIXME should use the log 
    std::cout <<"EventLoop create " << this << "in thread "<<threadId_<<std::endl;
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
        debug("Loop start!"); 
        activeChannels_.clear();
        poller_->poll(kPollTimeMs,&activeChannels_);  //利用poll来获取 activeChannels
        for(ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end() ; ++it)
        {
            (*it) -> handleEvent();
        }
    }
    //FIXME use the log
    std::cout <<" Loop Stop "<<std::endl;
    looping_ = false;
}
void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel * channel )
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_ -> updateChannel(channel);
}


//debug
void EventLoop::debug(  std::string  const  s)
{
    std::cout<< s << std::endl;
}















