/*************************************************************************
    > File Name: Acceptor.h
    > Author: 
    > Mail: 
    > Created Time: 2018年02月01日 星期四 20时47分10秒
 ************************************************************************/

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include"Channel.h"
#include"Socket.h"
#include<functional>
namespace Happy
{

class EventLoop;
class InetAddress;

class Acceptor 
{
public:
    typedef  std::function<void (int sockfd,const InetAddress& )> NewConnectionCallback;
    Acceptor(EventLoop * loop,const InetAddress& listenAddr);

    //设置回调函数
    void setNewConnectionCallback(const NewConnectionCallback& Callback)
    { newConnectionCallback_ = Callback; }
    
    //监听 
    void listen();
    bool listenning() const {return listenning_;}

    
private:
    void handleRead();    //供Channel调用
    EventLoop * loop_;    //每个Acceptor属于一个Eventloop
    Socket acceptSocket_; //Socket,其为Server Socket
    Channel acceptChannel_;//一个Channel,负责管理Server Socket
    NewConnectionCallback newConnectionCallback_; //出现新链接时的回调,供TcpServer new connection使用
    bool listenning_;
};



}





#endif
