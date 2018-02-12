/*************************************************************************
	> File Name: TcpServer.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月03日 星期六 10时23分39秒
 ************************************************************************/

#include"TcpServer.h"
#include"Acceptor.h"
#include"EventLoop.h"
#include"Socket_Base.h"
#include<functional>

#include<boost/bind.hpp>
#include<stdio.h>


using namespace Happy;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
  : loop_(loop) ,//FIXME 
    name_(listenAddr.toHostPort()),
    acceptor_(new Acceptor(loop, listenAddr)),
    started_(false),
    nextConnId_(1)

{
    //Acceptor 的回调为新建一个TcpConnection
    acceptor_->setNewConnectionCallback(
            boost::bind(&TcpServer::newConnection, this, _1,_2));
}

TcpServer::~TcpServer()
{

}


void TcpServer::start()
{
    if(!started_)
    {
        started_ = true;
    }
    
    //监听本地socket
    if(!acceptor_->listenning())
    {
        loop_->runInLoop(
        boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}


void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    loop_->assertInLoopThread();

    //获取TcpConnection名字
    char buf[32];
    snprintf(buf,sizeof buf,"#%d",nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;
    
    //debug
    std::cout<<"TcpServer::newConnection "<<name_<<" new Connection " <<connName 
             <<"] from" <<peerAddr.toHostPort()<<std::endl;

    //获取本机地址
    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    TcpConnectionPtr conn(
        new TcpConnection(loop_ , connName, sockfd,localAddr,peerAddr));
    
    //加入到Map映射
    connections_[connName] = conn; 
   
    //设置回调,这些回调会传给TcpConnection ,然后TcpConnection会传给Channel,然后对事件进行响应
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(
        boost::bind(&TcpServer::removeConnection, this,_1));

    conn->connectEstablished();
}


//删除TcpConnection
void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    
    size_t n = connections_.erase(conn->name());
    assert(n==1);
    loop_->queueInLoop(
        boost::bind(&TcpConnection::connectDestroyed,conn));
}











