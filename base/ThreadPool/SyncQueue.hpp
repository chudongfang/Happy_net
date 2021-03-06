#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>


template <typename T>

class SyncQueue
{
public:
    
    //构造函数
    SyncQueue(int maxSize) : maxSize_(maxSize) , needStop_(false)
    {

    }
    
    //左值放入
    void Put(const T& x)
    {
        Add(x);
    }

    //右值引用放入
    void Put(T&&x)
    {
        Add(std::forward<T>(x)); 
    }

    void Take(std::list<T>& list)
    {
        //配合条件变量使用
        std::unique_lock<std::mutex> locker(mutex_);

        notEmpty_.wait(locker, [this]{ return needStop_ || NotEmpty(); }  ); //等待满足条件 + lambda表达式
        if(needStop_)
            return ;
        list =  std::move(queue_); // 右值赋值,不需要进行拷贝
        notFull_.notify_one();
    }

    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(mutex_);

        notEmpty_.wait(locker, [this]{ return needStop_ || NotEmpty(); }  );

        if(needStop_)
            return ;
        
        t = queue_.front(); //左值赋值
        queue_.pop_front();
       
        notFull_.notify_one();//唤醒一个等待的线程
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> locker(mutex_);
            needStop_ = true;
        }
        notFull_.notify_all();//唤醒所有的进程
        notEmpty_.notify_all();//唤醒所有的进程
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

/*
    int Count()
    {
        return queue_.size();
    }
*/

private:

    bool NotFull() const 
    {
        bool full = queue_.size()  >= maxSize_;
        if(full)
        {
            std::cout << "The queue is full , Please wait! "<<std::endl;
        }
        return !full;
    }

    bool NotEmpty() const 
    {
        bool empty = queue_.empty();
        if(empty)
        {
            std::cout <<"The queue is empty , Please wait !" << std::this_thread::get_id() << std::endl;
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



