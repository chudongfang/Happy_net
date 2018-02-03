/*************************************************************************
	> File Name: Callbacks.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 00时50分46秒
 ************************************************************************/

#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#include<boost/shared_ptr.hpp>
#include<functional>

namespace Happy
{
class TcpConnection;

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&,
                              const char* data,
                              ssize_t len)> MessageCallback;
}

#endif
