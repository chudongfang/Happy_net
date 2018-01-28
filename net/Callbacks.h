/*************************************************************************
	> File Name: Callbacks.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 00时50分46秒
 ************************************************************************/

#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>

namespace Happy
{
    typedef boost::function<void()> TimerCallback;
}

#endif
