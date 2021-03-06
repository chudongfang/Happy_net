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

//class Channel : boost::noncopyable
class Channel
{
public:
    typedef boost::function<void()> EventCallback; //回调函数
    Channel(EventLoop * loop,int fd);
   
    ~Channel();//析构


    //最重要的函数,用来调配回调函数
    void handleEvent();

    
    //获取变量值 
    int fd() const {return fd_;}
    int events() const {return events_; }
    int index() const {return index_;}
    EventLoop * ownerLoop() {return loop_;}
    
    //设置变量值
    void set_index(int idx) {index_ = idx;} 
    void set_revents(int revt) { revents_ = revt; }
    
    //设置回调函数
    void setReadCallback(const EventCallback& cb)
    {readCallback_  = cb; }
    void setWriteCallback(const EventCallback& cb)
    {writeCallback_ = cb; }
    void setErrorCallback(const EventCallback& cb)
    {errorCallback_ = cb; }
    
    void setCloseCallback(const EventCallback& Callback) //设置关闭链接回调
    {
        closeCallback_ = Callback;
    }
    //修改fd为可读
    void enableReading() { events_ |= kReadEvent; update(); }
    
    //修改状态为可写 
    void enableWriting() { events_ |= kWriteEvent; update(); }
    //修改状态为不可写
    void disableWriting() { events_ &= ~kWriteEvent; update(); }



    //修改fd状态为0
    void disableAll() { events_ = kNoneEvent; update(); }
   
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isNoneEvent() const {return events_ == kNoneEvent;}
     
     
private:
    
    void update();
    
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;    //Channel对应的Eventloop 
    const int  fd_;    
    int        events_;
    int        revents_;
    int        index_;  //引索
    
    // 定义一些回调函数
    
    bool IsHandling_; 
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    
    EventCallback closeCallback_;// 关闭链接回调
    
};



}








#endif
