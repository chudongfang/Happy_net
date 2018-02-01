#include"./ThreadPool.hpp"

#include<iostream>
using namespace std;


int main()
{

    ThreadPool pool(2);
    
    std::thread thd1([&pool]
        {
            for(int i = 0 ;i < 1000; i++ )
            {
                auto thdId = this_thread::get_id();
                pool.AddTask([thdId]{
                    cout << "同步层线程1的ID:" << thdId << endl;   
                });
            }
        }
        
    );
    std::thread thd2([&pool]
        {
            for(int i = 0 ;i < 10; i++ )
            {
                auto thdId = this_thread::get_id();
                pool.AddTask([thdId]{
                    cout << "同步层线程2的ID:" << thdId << endl;   
                });
            }
        }
        
    );
    this_thread::sleep_for(std::chrono::seconds(4));
    getchar();
    pool.Stop();
    thd1.join();
    thd2.join();

    
    return 0;
}
