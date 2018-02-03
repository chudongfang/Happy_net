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
    channel_->setReadCallback(
        boost::bind(&TcpConnection::handleRead,this));
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


void TcpConnection::handleRead()
{
    //读取数据
    char buff[65536];
    ssize_t n = ::read(channel_->fd(),buff,sizeof(buff));
    messageCallback_(shared_from_this(),buff,n);
    //FIXME :close connection if n == 0
}















