

/*
 *struct pollfd{
    int fd;              //文件描述符
　　short events;    //请求的事件
　　short revents;   //返回的事件
　　};
*/


#include "Poller.h"
#include "Channel.h"

#include <assert.h>
#include <poll.h>
using namespace Happy;

Poller::Poller(EventLoop* loop)
 : ownerLoop_(loop)
 {
 }

Poller::~Poller()
{
}


//FIXME  可以把时间戳加上
int  Poller::poll(int timeoutMs, ChannelList * activeChannels)
{
    int numEvents = ::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
    //Timestap now(Timestap::now());
    if(numEvents>0)
    {
        cout << numEvents<<" events happended";
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents == 0 )
    {
        cout << "Nothing happended"<<endl;
    }
    else 
    {
        cout <<"Poller::poll()"<<endl;
    }
    return 0;
}
//把相应的事件放入 activeChannels
void Poller::fillActiveChannels(int numEvents,                    
                               ChannelList * activeChannels) const
{
    //O(n)时间复杂度循环遍历
    for(PollFdList::const_iterator  it = pollfds_.begin();
       it != pollfds_.end() && numEvents > 0 ; ++ it)
    {
        if( it->revents > 0)
        {
            --numEvents;
            ChannelMap::const_iterator ch = Channels_.find(it->fd);
            assert(ch != channels_.end()); // 验证对应的fd是否存在
            Channel* channel = ch->second;    
            assert(channel -> fd() == it->fd);  //验证fd是否对应
            channel->set_revents(it->revents);  //设置fd对应的revents
            activeChannels->push_back(channel);
        }
    }
}


void Poller::updateChannel(Channel* channel)
{
    assertInLoopThread();
    cout << "fd = " <<channel->fd()<<" events = "<<channel->events();
    if(channel->index() < 0)
    {
        //一个新的fd,添加到pollfds_
        assert(Channels_.find(channel->fd())  == channels_.end());
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);
        int idx = static_cast<int>(pollfds_.size()) - 1;
        channel->set_index(idx);
        channels_[pfd.fd] = channel;
    }
    else {
        //fd已经存在
        assert(channels_.find(channel->fd()) != channels_.end() );
        assert(channels_[channel->fd()] == channel);
        int idx = channel->index();
        assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
        struct pollfd & pfd = pollfd_[idx];
        assert(pfd.fd == channel->fd() || pfd.fd == -1);
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent())
        {
            pfd.fd = -1;
        }
    }
}









