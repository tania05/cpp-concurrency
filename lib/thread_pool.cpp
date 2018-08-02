// #include <iostream>uh 
#include "ra/thread_pool.hpp"
// #include "ra/queue.hpp"
// #include <thread>
// #include <iostream>
// #include <cstdlib>
// #include <vector>

namespace ra:: concurrency {
// Thread pool class.

  thread_pool::thread_pool (): thread_no(2), _queue(32), _shutdown(false)
  {
    if(std::thread::hardware_concurrency() != 0)
    {
        thread_no = std::thread::hardware_concurrency();
    }

    for(std::size_t i =0; i< thread_no; ++i)
    {
        std::unique_ptr<std::thread> newThread = std::make_unique<std::thread>(&ra::concurrency::thread_pool::make_thread, this);
        pool.push_back(std::move(newThread));
    }
  }

  
  // Creates a thread pool with num_threads threads.
  // Precondition: num_threads > 0
  thread_pool::thread_pool ( std::size_t num_threads ) : thread_no(num_threads), _queue(32), _shutdown(false)
  {
    for(std::size_t i =0; i< thread_no; ++i)
    {
        std::unique_ptr<std::thread> newThread = std::make_unique<std::thread>(&ra::concurrency::thread_pool::make_thread, this);
        pool.push_back(std::move(newThread));
    }
  }

  // Destroys a thread pool, shutting down the thread pool first
  // (if not already shutdown).
  thread_pool::~thread_pool ()
  {
    shutdown();
    // delete &thread_pool::pool;
  }
  
  // Gets the number of threads in the thread pool.
  // This function is not thread safe.
  thread_pool::size_type thread_pool::size () const
  {
    return pool.size();
  }
  
  // Enqueues a task for execution by the thread pool.
  // The task to be performed is the code associated with the
  // callable entity func.
  // Note: The rvalue reference parameter is intentional and
  // implies that the schedule function is permitted to change
  // the value of func (e.g., by moving from func).
  // Precondition: The thread pool is not in the shutdown state.
  // This function is thread safe.
  void thread_pool::schedule ( std::function <void() >&& func )
  {
    std::unique_lock lock(_m);
    _cv.wait(lock, [this]() {return !_queue.is_full() || _shutdown ; });
    if(!_shutdown)
    {
      _queue.push(std::move(func));
    }
    lock.unlock();
    _cv.notify_one();
  }
  
  // Shuts down the thread pool.
  // This function does not return until all threads have
  // completed their tasks.
  // If the thread pool is already shutdown, this function has
  // no effect.
  // After the thread pool is shutdown, it can only be destroyed.
  // This function is thread safe.
  void thread_pool::shutdown ()
  {
    std::unique_lock lock(_m);
    _shutdown = true;
    lock.unlock();
    _cv.notify_all();
    for(auto & i : pool)
    {
      i->join();
    }
  }
  
  // Tests if the thread pool has been shutdown.
  // This function is not thread safe.
  bool thread_pool::is_shutdown () const
  {
    return _shutdown;
  }

  void thread_pool::make_thread()
  {
    while(true)
    {
      std::unique_lock lock(_m);
      _cv.wait(lock, [this] () {return !_queue.is_empty() || _shutdown;});
      
      if(_shutdown)
      {
        //the pool is shut down, don't pop or do anything, immediately return
        lock.unlock();
        _cv.notify_one();
        return;
      }

      std::function<void()> val;
      _queue.pop(val);

      lock.unlock();
      _cv.notify_one();
      if(val)
      {
        val();
      }
    }
  }
  
}
