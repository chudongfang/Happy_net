/*************************************************************************
	> File Name: Thread.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 12时02分00秒
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H

#include "Atomic.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace Happy
{
class Thread : boost::noncopyable
{
public:
    typedef boost::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    
    void start();
    void join();

    bool started(){}



private:
    bool started_;
    bool joined_;
    std::string name_; //名字
    ThreadFunc func_;
    pthread_t pthreadId_;
    boost::shared_ptr<pid_t> tid_;



}
}


#endif
