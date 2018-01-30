/*************************************************************************
	> File Name: EventLoop.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月11日 星期四 18时00分06秒
 ************************************************************************/

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
#include <boost/scoped_ptr.hpp>
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>

#include <thread1/CurrentThread.h>

namespace Happy
{

class Channel;
class Poller;
//FIXME
//class EventLoop : boost::noncopyable
class EventLoop 
{
public:
    
    typedef std::function<void()> Functor;
    
    EventLoop();//structure function
    
    ~EventLoop();
    
    
    void loop();
    void quit();
   
    //
    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);

    void wakeup();
    //

    void updateChannel(Channel* channel);
    
    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();} //判断是否是当前的线程
    void debug(std::string s);
    



private:
    void abortNotInLoopThread(); 
    
    void handleRead();
    void doPendingFunctors();

    
     
    
    typedef std::vector<Channel*> ChannelList; //存储Channel
    
    bool looping_;
    bool quit_;
    bool callingPengdingFunctors_;
    
    const pid_t threadId_;  //线程ID
    boost::scoped_ptr<Poller> poller_; //禁止拷贝与赋值的智能指针
    ChannelList activeChannels_; //活动Channel列表 
    int wakeupFd_;
    boost::scoped_ptr<Channel> wakeupChannel_;
    
    //FIXME
    std::mutex mutex_;

    std::vector<Functor> pendingFunctors_;

};


}









#endif
