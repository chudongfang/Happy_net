#include "Channel.h"
#include "Poller.h"
#include "EventLoop.h"


#include <boost/bind.hpp>

#include <sys/eventfd.h>
#include <iostream>
#include <assert.h>
using namespace Happy;

__thread EventLoop* t_loopInThisThread =0 ;


//创建Eventfd
static int createEventfd()
{
    int Eventfd = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
    if(Eventfd < 0)
    {
        std::cout << " Failed in Eventfd !" <<std::endl;
        abort();
    }
    return Eventfd;
}




const int kPollTimeMs = 100000; //poll等待秒数
EventLoop::EventLoop()
    :looping_(false),
    quit_(false),
    callingPengdingFunctors_(false),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this,wakeupFd_)),
    threadId_(CurrentThread::tid()), 
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
    wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead,this));    //FIXME c++11
    wakeupChannel_->enableReading(); //修改状态为可读并加入到pollfd
}


EventLoop::~EventLoop()
{
    assert(!looping_);
    ::close(wakeupFd_);//关闭wakeupFd_
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
            (*it) -> handleEvent();  //根据事件的不同执行对应的回调
        }

        doPendingFunctors();

    }
    //FIXME use the log
    std::cout <<" Loop Stop "<<std::endl;
    looping_ = false;
}
void EventLoop::quit()
{
    quit_ = true;
    if(!isInLoopThread())
    {
        wakeup();
    }
}

void EventLoop::updateChannel(Channel * channel )
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    
    poller_ -> updateChannel(channel);
}


//删除Channel,调用Poller 中的removeChannel
void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}






void EventLoop::abortNotInLoopThread()
{
    std::cout<<threadId_<<" "<<CurrentThread::tid()<<std::endl;
}


//debug
void EventLoop::debug(  std::string  const  s)
{
    std::cout<< s << std::endl;
}



void EventLoop::runInLoop(const Functor & Callback)
{
    //如果在当前线程就直接执行
    if(isInLoopThread())
    {
        Callback();
    }
    else
    {
        queueInLoop(Callback);
    }
}

void EventLoop::queueInLoop(const Functor & Callback)
{
    {
        std::lock_guard<std::mutex> locker(mutex_);
        //FIXME use the lock
        pendingFunctors_.push_back(Callback);
    }
    if(!isInLoopThread() || callingPengdingFunctors_)
    {
        wakeup();
    }
}


void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_,&one,sizeof(one));
    if(n != sizeof(one))
    {
        std::cout<<"Error: write wakeupFd_!"<<std::endl;
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_,&one,sizeof(one));
    if(n != sizeof(one))
    {
        std::cout<<"Error: read wakeupFd_!"<<std::endl;
    }
}


void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPengdingFunctors_ = true;
    {
        //FIXME use the Mutex
        std::lock_guard<std::mutex> locker(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(size_t i = 0 ; i < functors.size(); i++)
    {
        functors[i]();
    }

    callingPengdingFunctors_ = false;
}


