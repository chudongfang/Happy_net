/*************************************************************************
	> File Name: Acceptor.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 21时03分33秒
 ************************************************************************/

#include"Acceptor.h"
#include"EventLoop.h"
#include"InetAddress.h"
#include"Socket_Base.h"
#include<boost/bind.hpp>

using namespace Happy;


Acceptor::Acceptor(EventLoop* loop , const InetAddress& listenAddr)
    : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie()), //初始化一个非阻塞Socket
    acceptChannel_(loop, acceptSocket_.fd()),         //初始化Channel
    listenning_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);//给本机Socket绑定地址
    acceptChannel_.setReadCallback(
        boost::bind(&Acceptor::handleRead, this));
}


void Acceptor::listen() //调用listen函数监听本机Socket
{
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}


void Acceptor::handleRead()//作为Channel的回调,当Sockets出现新连接的情况下进行Acceptor
{
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);
    int connfd = acceptSocket_.accept(&peerAddr);
    //创建新的TcpConnection
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } 
        //如果没有设置回调函数,则关闭Socket
        else  
        {
            sockets::close(connfd);
        }
    }
  

}
