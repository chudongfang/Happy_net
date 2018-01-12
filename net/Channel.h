/*************************************************************************
	> File Name: Channel.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月11日 星期四 18时22分51秒
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H
#include<boost/function.hpp>
#include<boost/noncopyable.hpp>

namespace Happy
{
class EventLoop;

class Channel : boost::noncopyable
{
  public:
    typedef boost::function<void()> EventCallback;
    Channel(EventLoop * loop,int fd);
    void handleEvent();
    void setReadCallback(const EventCallback& cb)
    {readCallback_  = cb; }
    void setWriteCallback(const EventCallback& cb)
    {writeCallback_ = cb; }
    void setErrorCallback(const EventCallback& cb)
    {errorCallback_ = cb; }
 private:
    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int  fd_;
    int        events_;
    int        revents_;
    int        index_;
    
    // 定义一些回调函数
    
    
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;


};



};








#endif