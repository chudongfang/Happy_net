/*************************************************************************
	> File Name: InetAddress.h
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 20时10分24秒
 ************************************************************************/

#ifndef _INETADDRESS_H
#define _INETADDRESS_H
#include<string>
#include<netinet/in.h>

namespace Happy
{
    
class InetAddress 
{
public:
    explicit InetAddress(uint16_t port);

    InetAddress(const std::string& ip, uint16_t port);

    InetAddress(const struct sockaddr_in& addr)
        : addr_(addr)
    { }
    
    std::string toHostPort() const ;

    const struct sockaddr_in& getSockAddrInet() const { return addr_; }

    void setSockAddrInet(const struct sockaddr_in& addr) {addr_=  addr;};
private:
    
    struct sockaddr_in addr_;



};



}


#endif