/*************************************************************************
	> File Name: EventLoop.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月11日 星期四 18时00分06秒
 ************************************************************************/

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
#include "../base/Thread"

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
    void abortNotInLoopThread();
    typedef std::vector<Channel*> ChannelList;
    bool looping_;
    bool quit_;
    const pid_t threadId_;
    boost::scoped_ptr<Poller> poller_;
    ChannelList activeChannels;

};


}









#endif
