#include "EventLoop.h"
#include <thread>
#include <stdio.h>

void threadFunc()
{
  printf("threadFunc(): pid = %d, tid = %d\n",
         getpid(), Happy::CurrentThread::tid());

  Happy::EventLoop loop;
  loop.loop();
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), Happy::CurrentThread::tid());

  Happy::EventLoop loop;

    std::thread my_thread(threadFunc);

  loop.loop();
  return 0;
}
