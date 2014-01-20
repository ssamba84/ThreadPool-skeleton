#ifndef THREADPOOL_H
#define THREADPOOL_H

# include <functional>		
# include <thread>		
# include <mutex>		
# include <condition_variable>	
# include <vector>			
# include <deque>			
# include <atomic>			
 

using namespace std;

typedef function<void()> Task;

class ThreadPool{

  public:
   /*
      ThreadPool constructor

      size - number of threads to start with
    */
   ThreadPool(int size);

   /* 
      addWork
      
      enqueue task to work queue and signal IF thread is not shutting down 
    */
   bool addWork(const Task & task);
 
   /*
      shutdown

      set shutdown flag and signal
    */
   bool shutdown();

   /*
      doWork

      main worker thread
    */
   void doWork();

  private:
   mutex  			_mut_q;		// to protect the work queue
   condition_variable 		_cond_mut_q;	// signal work queue state
   deque<Task> 			_work_queue;	// queue of function ptrs to be processed by threads

   vector<thread> 		_thread_list;	// threadid -> thread

   atomic<bool>			_shutdown;	// flag is set if thread pool is to be brought down
   
   int 				_size;		// no of threads
   
};

#endif
