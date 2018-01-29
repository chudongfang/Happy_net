/*************************************************************************
	> File Name: CurrentThread.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 18时15分46秒
 ************************************************************************/

#ifndef _CURRENTTHREAD_H
#define _CURRENTTHREAD_H

#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace Happy
{


namespace CurrentThread
{
  pid_t tid();
  const char* name();
  bool isMainThread();
}




}



#endif
