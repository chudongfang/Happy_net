/*************************************************************************
	> File Name: test_Acceptor.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 21时17分26秒
 ************************************************************************/

#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "./Socket_Base.h"
#include <stdio.h>

void newConnection(int sockfd, const Happy::InetAddress& peerAddr)
{
  printf("newConnection(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  ::write(sockfd, "How are you?\n", 13);
  Happy::sockets::close(sockfd);
}

int main()
{
  printf("main(): pid = %d\n", getpid());

  Happy::InetAddress listenAddr(12345);
  Happy::EventLoop loop;

  Happy::Acceptor acceptor(&loop, listenAddr);
  acceptor.setNewConnectionCallback(newConnection);
  acceptor.listen();

  loop.loop();
  return 0;
}
