#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>


template <typename T>

class SyncQueue
{
public:
    SyncQueue(int maxSize) : maxSize_(maxSize) , needStop_(false)
    {

    }
    
    void Put(const T& x)
    {
        Add(x);
    }

    void Put(T&&x)
    {
        Add(std::forward<T>(x)); 
    }

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(mutex_);

        notEmpty_.wait(locker, [this]{ return needStop_ || NotEmpty(); }  );
        if(needStop_)
            return ;
        list =  std::move(queue_);
        notFull_.notify_one();

    }

    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(mutex_);

        notEmpty_.wait(locker, [this]{ return needStop_ || NotEmpty(); }  );
        if(needStop_)
            return ;
        
        t = queue_.front();
        queue_.pop_front();
       
        notFull_.notify_one();
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> locker(mutex_);
            needStop_ = true;
        }
        notFull_.notify_all();
        notEmpty_.notify_all();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.empty();
    }
    
    bool Full()
    {
        std::lock_guard<std::mutex> locker(mutex_);
        return  queue_.size() == maxSize_;
    }
    
    size_t Size()
    {
        
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.size();
    }


    int Count()
    {
        return queue_.size();
    }


private:

    bool NotFull() const 
    {
        bool full = queue_.size()  >= maxSize_;
        if(full)
        {
            std::cout << "is full , please wait! "<<std::endl;
        }
        return !full;
    }

    bool NotEmpty() const 
    {
        bool empty = queue_.empty();
        if(empty)
        {
            std::cout <<"empty , need wait !" << std::this_thread::get_id() << std::endl;
        }

        return !empty;

    }

    
    template <typename F>
    void Add(F&&x)
    {
        std::unique_lock<std::mutex> locker(mutex_);
        notFull_.wait(locker, [this]{return needStop_ || NotFull();} );
        if(needStop_)
            return ;
        queue_.push_back(std::forward<F>(x));
        notEmpty_.notify_one();
    }



private:
    std::list<T> queue_;
    std::mutex mutex_;// 互斥锁
    std::condition_variable notEmpty_; //非空条件变量
    std::condition_variable notFull_;  //非满条件变量
    int maxSize_;
    bool needStop_;
};



