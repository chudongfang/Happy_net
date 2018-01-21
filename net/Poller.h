/*************************************************************************
	> File Name: Poller.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月13日 星期六 00时41分12秒
 ************************************************************************/

#ifndef _POLLER_H
#define _POLLER_H
#include<map>
#include<vector>

#include"EventLoop.h"

struct pollfd;

namespace Happy
{

class Channel;

class Poller : boost::noncopyable
{
  public:
    typedef std::vector<Channel *> ChannelList;
    Poller(EventLoop* loop);
    ~Poller();

    void updateChannel(Channel* channel);




  private:
    void fillActiveChannels(int numEvents,
                           ChannelList * activeChannels) const ;
    typedef std::vector<struct pollfd>  PollFdList;
    typedef std::map<int , Channel*> ChannelMap;

    EventLoop * ownerLoop_;
    PollFdList  pollfds_;
    ChannelMap  channels_;
}

}








#endif
