/*************************************************************************
	> File Name: TcpConnection.h
	> Author: 
	> Mail: 
	> Created Time: 2018年02月03日 星期六 01时01分37秒
 ************************************************************************/

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include"InetAddress.h"

#include<boost/any.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<boost/scoped_ptr.hpp>
#include<boost/shared_ptr.hpp>
#include<functional>


namespace Happy
{
class Channel;
class EventLoop;
class Socket;


class TcpConnection 
{
public:
    typedef function<void (const TcpConnectionPtr&)> ConnectionCallback;
    typedef function<void (const TcpConnectionPtr&,
                              const char* data,
                              ssize_t len)> MessageCallback;
    
    TcpConnection(EventLoop* loop,
                  const std::string& name;
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr);

    ~TcpConnection();

    EventLoop* getLoop() const { return loop_; }
    const std::string& name() const { return name_; }
    const InetAddress& localAddress() {return localAddr_; }
    const InetAddress& peerAddress()  { return peerAddr_; }
    bool connected() const { return state_ == kConneted; }
    
    void setConnectionCallback(const ConnectionCallback& Callback)
    {
        connectionCallback_ = Callback;
    }

    void setMessageCallback(const MessageCallback& Callback)
    {
        messageCallback_  = Callback;
    }

    //只能调用一次
    void connectEstablished();


private:
    enum StateE{ kConneting, kConneted, };

    void setState(StateE s) {  }
    void handleRead();

    EventLoop * loop_;
    std::string name_;
    StateE state_;
    boost::scoped_ptr<Socket> socket_;
    boost::scoped_ptr<Channel> channel_;
    InetAddress localAddr_; //本机地址
    InetAddress peerAddr_;  //客户端地址
    ConnectionCallback connectionCallback_;
    MessageCallback    messageCallback_;
    

};
    
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;





}




#endif
