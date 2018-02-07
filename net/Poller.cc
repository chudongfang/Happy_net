

/*
 *struct pollfd{
    int fd;              //文件描述符
　　short events;    //请求的事件
　　short revents;   //返回的事件
　　};
*/


#include "Poller.h"
#include "Channel.h"

#include <iostream>
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
        std::cout << numEvents<<" events happended";
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents == 0 )
    {
        std::cout << "Nothing happended"<<std::endl;
    }
    else 
    {
        std::cout <<"Poller::poll()"<<std::endl;
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
            ChannelMap::const_iterator ch = channels_.find(it->fd);
            assert(ch != channels_.end()); // 验证对应的fd是否存在
            Channel* channel = ch->second;    
            assert(channel -> fd() == it->fd);  //验证fd是否对应
            channel->set_revents(it->revents);  //设置fd对应的revents
            activeChannels->push_back(channel);
        }
    }
}



//添加或修改pollfds_的信息,把Channel中的变化更新到poll中
void Poller::updateChannel(Channel* channel)
{
    assertInLoopThread();
    std::cout << "fd = " <<channel->fd()<<" events = "<<channel->events()<<std::endl;
    if(channel->index() < 0)
    {
        //一个新的fd,添加到pollfds_
        assert(channels_.find(channel->fd())  == channels_.end());
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
        struct pollfd & pfd = pollfds_[idx];
        
        assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);

        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent())
        {
            pfd.fd = -channel->fd()-1;
        }
        
    }
}


void Poller::removeChannel(Channel* channel)
{

    assertInLoopThread();
    assert(channels_.find(channel->fd()) != channels_.end() );
    assert(channels_[channel->fd()] == channel);
    assert(channel->isNoneEvent());

    int idx = channel->index();
   
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    
    //得到其对应的pollfd
    const struct pollfd& pfd = pollfds_[idx] ;
    assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
    size_t n = channels_.erase(channel->fd());
    assert(n == 1); 
    
    //删除其对应的pollfds
    if (implicit_cast<size_t>(idx) == pollfds_.size()-1) {
        pollfds_.pop_back();
    }
    else 
    {
        int channelAtEnd = pollfds_.back().fd;
        iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
        if (channelAtEnd < 0) {
            channelAtEnd = -channelAtEnd-1;
        }
        channels_[channelAtEnd]->set_index(idx);
        pollfds_.pop_back();
   }



}













