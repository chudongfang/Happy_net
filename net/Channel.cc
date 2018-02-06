#include "Channel.h"
#include "EventLoop.h"

#include<sstream>
#include<poll.h>


using namespace Happy;

//fd的状态
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fdArg)
 : loop_(loop),
    fd_(fdArg),
    events_(0),
    revents_(0),
    index_(-1),
    IsHandling_(false)
{
}



Channel::~Channel()
{
    assert(!IsHandling_);
}


void Channel::update()
{
    loop_->updateChannel(this);//use the poller

}


void Channel::handleEvent()
{

    IsHandling_ = true;
    if(revents_ & POLLNVAL)
    {
       // std::cout <<"POLLNVAL"<<std::endl;
    }


    //关闭链接
    if( (revents_ & POLLHUP) && !(revents_ & POLLIN) )
    {
        if(closeCallback_) closeCallback_();
    }

    if(revents_ & (POLLERR | POLLNVAL) ){
        if(errorCallback_) errorCallback_();
    }

    //出现可读事件时调用readCallback_()回调
    //这里可用Acceptor的回调,用来Accept Socket
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP) ){
        if(readCallback_) readCallback_();
    }
    
    if(revents_ & (POLLOUT) ){
        if(writeCallback_) writeCallback_();
    }
    IsHandling_ = false;
}











