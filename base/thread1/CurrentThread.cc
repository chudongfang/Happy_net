#include "CurrentThread.h"


namespace muduo
{
namespace CurrentThread
{
  __thread const char* t_threadName = "unknown";
}
}


using namespace Happy;

pid_t CurrentThread::tid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

const char* CurrentThread::name()
{
  return t_threadName;
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}
