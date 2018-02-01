/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
 ************************************************************************/

#include<chrono>
#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

mutex mutex_;

void func()
{
    std::lock_guard<std::mutex> locker(mutex_);
    cout << "thread " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::seconds(1));
    
}


int main()
{

    thread t3(func);
    thread t1(func);
    thread t2(func);
    t1.join();
    t2.join();
    t3.join();
    return 0;
    
}
