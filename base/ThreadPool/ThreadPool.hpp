#include<list>
#include<thread>
#include<functional>
#include<memory>
#include<atomic>
#include"SyncQueue.hpp"

const int MaxTaskCount = 100;

class ThreadPool
{
public:
    using Task = std::function<void()> ;
    ThreadPool(int numThreads = std::thread::hardware_concurrency()) : queue_(MaxTaskCount)
    {
        Start(numThreads);
    }

    ~ThreadPool(void)
    {
        Stop();
    }
    
    void Stop()
    {
        std::call_once(flag_, [this] {StopThreadGroup();});
    }

    void AddTask(const Task & task)
    {
        queue_.Put(task);
    }

private:
    void Start(int numThreads)
    {
        running_ = true;
        for(int i = 0;i < numThreads ;i++)
        {
            threadgroup_.push_back( std::make_shared<std::thread>(&ThreadPool::RunInThread, this) );
        }
    }

    void RunInThread()
    {
        while(running_)
        {
            std::list<Task> list;
            queue_.Take(list);
            for(auto & task : list)
            {
                if(!running_)
                {
                    return; 
                }
                task();
            }
        }
    }
    
    void StopThreadGroup()
    {
        queue_.Stop();
        running_ = false;
        for(auto thread : threadgroup_)
        {
            if(thread)
                thread->join();
        }
        threadgroup_.clear();
    }

private:

    std::list<std::shared_ptr<std::thread>> threadgroup_;
    SyncQueue<Task> queue_;
    std::atomic_bool running_;
    std::once_flag flag_;

};
