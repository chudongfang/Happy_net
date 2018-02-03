/*************************************************************************
	> File Name: TcpServer.h
	> Author: 
	> Mail: 
	> Created Time: 2018年02月03日 星期六 00时58分24秒
 ************************************************************************/

#ifndef _TCPSERVER_H
#define _TCPSERVER_H


#include"Callbacks.h"
#include"TcpConnection.h"

#include<map>
#include<boost/scoped_ptr.hpp>

namespace Happy
{

class Acceptor;
class EventLoop;

class TcpServer
{
public:
    TcpServer(EventLoop* loop,const InetAddress& listenAddr);
    ~TcpServer();
    
    void start();

    //设置回调
    void setConnectionCallback(const ConnectionCallback& Callback)
    {
        connectionCallback_ = Callback;
    }
    
    void setMessageCallback(const MessageCallback& Callback)
    {
        messageCallback_ = Callback;
    }
    

private:
    void newConnection( int sockfd, const InetAddress& peerAddr );

    typedef std::map<std::string , TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string name_;
    boost::scoped_ptr<Acceptor> acceptor_;

    ConnectionCallback connectionCallback_;
    MessageCallback    messageCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;

};


}














#endif
