/*************************************************************************
	> File Name: Poller.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月28日 星期日 11时01分09秒
 ************************************************************************/

#ifndef _POLLER_H
#define _POLLER_H

#include <map>
#include <vector>
#include "EventLoop.h"

struct pollfd;

namespace Happy
{

class Channel;

//class Poller : boost::noncopyable
class Poller 
{
public :
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop * loop);
    ~Poller();

    int poll(int timeoutMs, ChannelList * activeChannels);

    void updateChannel(Channel * channel);
    void assertInLoopThread() {ownerLoop_ -> assertInLoopThread();}

private:
    void fillActiveChannels(int numEvents , ChannelList * activeChannels) const;
    
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int , Channel*> ChannelMap;
    EventLoop *  ownerLoop_; // 其对应的EvenLoop
    PollFdList   pollfds_;   //存储fd
    ChannelMap channels_;    //fd 与 channel对应

};
    

}







#endif
