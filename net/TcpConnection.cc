/*************************************************************************
	> File Name: TcpConnection.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月03日 星期六 01时22分10秒
 ************************************************************************/

#include"TcpConnection.h"
#include"Channel.h"
#include"EventLoop.h"
#include"Socket.h"
#include"Socket_Base.h"

#include<iostream>
#include<cerrno>
#include<cstdio>
#include<boost/bind.hpp>

using namespace Happy;

TcpConnection::TcpConnection(EventLoop* loop,
                             const std::string& name,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
  : loop_(loop),
    name_(name),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr)
{
    std::cout <<"Create TcpConnection"<<"name: "<<name_<<" at "<<this <<" fd = "<<channel_->fd()<<std::endl;

    //设置读数据回调,当有数据出现时读数据
    //FIXME use std C++11
    channel_->setReadCallback(
        boost::bind(&TcpConnection::handleRead, this));
    channel_->setCloseCallback(
        boost::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(
        boost::bind(&TcpConnection::handleError, this));

}


TcpConnection::~TcpConnection()
{
    std::cout <<"Delete TcpConnection"<<"name: "<<name_<<" at "<<this <<" fd = "<<channel_->fd()<<std::endl;
}


//建立链接,设置可读
void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);

    //设置Socket可读
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

//删除TcpConnection
//用在TcpServer , 供runInLoop调用
void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();

    assert(state_ == kConnected || state_ == kDisconnecting);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());
    loop_->removeChannel(get_pointer(channel_));
}



//当epoll产生读数据时的回调
//当链接断开时,则调用handleClose
void TcpConnection::handleRead()
{
    //读取数据
    char buff[65536];
    ssize_t n = ::read(channel_->fd(),buff,sizeof(buff));
    if( n > 0 )
    {
        messageCallback_(shared_from_this(),buff,n);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        handleError();
    }
}

void TcpConnection::handleWrite()
{

}


//关闭TcpConnection
void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}


//error回调
void TcpConnection::handleError()
{
    ;
}



//send为线程安全函数,首先判断其是否在本线程内,如果不在则调用runInLoop
void TcpConnection::send(const std::string&  message)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            loop_->runInLoop(
                boost::bind(&TcpConnection::sendInLoop,this,message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string& message)
{
    loop_->assertInLoopThread();
    ssize_t numwrite = 0;

    if(!channel_->isWriting())
    {
        numwrite = ::write(channel_->fd(),message.data(),message.size());
        if(numwrite >= 0)
        {
            if(numwrite < message.size())
            {
                std::cout<<"I am going to write more data!"<<std::endl;
            }
            else 
            {
                //FIXME  dell with the error
                numwrite = 0;
                if(errno != EWOULDBLOCK)
                {
                    ;
                }
            }
        }
    }

    //FIXME
}


void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setState(kDisconnected);
        loop_->runInLoop(
            boost::bind(&TcpConnection::shutdownInLoop, this));
    }
}


//在runInLoop中执行
void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
    {
        socket_->shutdownWrite();
    }
}








