# include <iostream>
# include "threadpool.h"
 
using namespace std;

ThreadPool::ThreadPool(int size):_size(size){
   _shutdown = false;
   for (int i = 0; i < size; ++i){
      cout<<"firing off a thread"<<endl;
      _thread_list.push_back(thread(&ThreadPool::doWork, this));
   } 
}

bool ThreadPool::addWork(const Task & task){
   if (!_shutdown){
      std::unique_lock<std::mutex> lck(_mut_q);
      _work_queue.push_back(task);
      _cond_mut_q.notify_one();
      cout<<"Added item on to work queue\n";
      return 1;
   }
      return 0;
}
 
bool ThreadPool::shutdown(){
   _shutdown  = true;
   _cond_mut_q.notify_one();
   for (auto & th : _thread_list) {
        th.join();
   }
}

void ThreadPool::doWork(){
   cout<<"inside doWork tid is "<<this_thread::get_id()<<endl;
   while(true){
      
      std::unique_lock<std::mutex> lck(_mut_q);
      while(_work_queue.empty() && !_shutdown){
         _cond_mut_q.wait(lck);
      } 
      if (_shutdown){
         _cond_mut_q.notify_one();
         lck.unlock();
         cout<<"shutting down "<<this_thread::get_id()<<endl;
         break;
      }
      Task t = _work_queue.front();
      _work_queue.pop_front();
      lck.unlock();
      t();
   }
   
}


