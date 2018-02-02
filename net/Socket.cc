#include "Socket.h"
#include "InetAddress.h"
#include "Socket_Base.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>


using namespace Happy;


Socket::~Socket()
{
    sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& addr)
{
    sockets::bindOrDie(sockfd_,addr.getSockAddrInet());
}


void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}


//Accept并得到客户端Socket地址保存在peeraddr中
int Socket::accept(InetAddress * peeraddr)
{
    struct sockaddr_in addr;
    bzero(&addr , sizeof addr);
    int connfd = sockets::accept(sockfd_, &addr);
    if ( connfd >= 0 )
    {
        peeraddr -> setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1: 0;
    
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof optval);
}








