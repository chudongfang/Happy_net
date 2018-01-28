/*************************************************************************
	> File Name: Callbacks.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月28日 星期日 18时11分15秒
 ************************************************************************/

#ifndef _CALLBACKS_H
#define _CALLBACKS_H
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace Happy
{
    typedef boost::function<void()> TimerCallback;
}

#endif
