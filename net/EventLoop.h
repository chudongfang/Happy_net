/*************************************************************************
	> File Name: EventLoop.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月11日 星期四 18时00分06秒
 ************************************************************************/

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include<boost/scoped_ptr.hpp>
#include<vector>

namespace Happy
{

class Channel;
class Poller;

class EventLoop : boost::noncopyable
{
public:
    EventLoop();//structure function
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* channel);
    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();}
  
    
private:
    void abortNotInLoopThread(); //判定是否在线程中

    typedef std::vector<Channel*> ChannelList; //存储Channel
    bool looping_;
    bool quit_;
    const pid_t threadId_;  //线程ID
    boost::scoped_ptr<Poller> poller_; //禁止拷贝与赋值的智能指针
    ChannelList activeChannels; //活动Channel列表 

};


}









#endif
