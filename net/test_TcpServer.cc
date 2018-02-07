/*************************************************************************
	> File Name: test_TcpServer.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月03日 星期六 11时50分33秒
 ************************************************************************/
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>

void onConnection(const Happy::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(),
           conn->peerAddress().toHostPort().c_str());
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
           conn->name().c_str());
  }
}

void onMessage(const Happy::TcpConnectionPtr& conn,
               const char* data,
               ssize_t len)
{
    if(len==0) return ;
  printf("onMessage(): received %zd bytes from connection [%s]\n",
         len, conn->name().c_str());
    conn->send("lalal~");
}

int main()
{
  printf("main(): pid = %d\n", getpid());

  Happy::InetAddress listenAddr(12345);
  Happy::EventLoop loop;

  Happy::TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  server.start();

  loop.loop();
}
