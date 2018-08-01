#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace ra::concurrency {
// Concurrent bounded FIFO queue class.
template <class T >
class queue
  {
    public:
    
      // The type of each of the elements stored in the queue.
      using value_type = T;
      
      // An unsigned integral type used to represent sizes.
      using size_type = std :: size_t ;
      
      // A type for the status of a queue operation.
      enum class status {
      success = 0, // operation successful
      empty , // queue is empty
      full , // queue is full
      closed , // queue is closed
      };
      
      // A queue is not default constructible.
      queue () = delete;
      
      // Constructs a queue with a maximum size of max_size.
      // The queue is marked as open (i.e., not closed).
      // Precondition: The quantity max_size must be greater than
      // zero.
      queue ( size_type max_size ) : _max_size(max_size), _closed(false)
      {
        &_queue = new std::queue<T>();
      }

      // A queue is not movable or copyable.
      queue (const queue &) = delete;
      queue & operator=(const queue &) = delete;
      queue ( queue &&) = delete;
      queue & operator=( queue &&) = delete;

      // Destroys the queue after closing the queue (if not already
      // closed) and clearing the queue (if not already empty).
      ~queue ()
      {
        close();
        clear();
        std::destroy(&_queue);
      }
      
      // Inserts the value x at the end of the queue, blocking if
      // necessary.
      // If the queue is full, the thread will be blocked until the
      // queue insertion can be completed.
      // If the value x is successfully inserted on the queue, the
      // function returns status::success.
      // If the value x cannot be inserted on the queue (due to the
      // queue being closed), the function returns with a return
      // value of status::closed.
      // This function is thread safe.
      // Note: The rvalue reference parameter is intentional and
      // implies that the push function is permitted to change
      // the value of x (e.g., by moving from x).

      //TODO: MAKE THREAD SAFE
      status push ( value_type && x )
      {
        std::unique_lock lock(_queue_mutex, std::defer_lock);
        
        _cv.wait(lock, [this] () {return !is_full() || _closed;});
        status result = status::success;
        
        if(_closed)
        {
          result = status::closed;
        }
        else
        {
          _queue.push(x);

        }
        lock.unlock();
        _cv.notify_one();
        return result;

      }
      
      // Removes the value from the front of the queue and places it
      // in x, blocking if necessary.
      // If a value is successfully removed from the queue, the value
      // is placed in x and the function returns status::success.
      // If the queue is empty and not closed, the thread will be
      // blocked until a value can be removed from the queue (or the
      // queue is later closed).
      // If the queue is closed, the function does not block and either
      // returns status::closed or status::success, depending on whether
      // a value can be successfully removed from the queue.
      // If a value cannot be successfully removed from the queue (due to
      // the queue being both empty and closed), the function returns
      // status::closed.
      // This function is thread safe.
      status pop ( value_type & x )
      {
        std::unique_lock lock(_queue_mutex);

        _cv.wait(lock, [this]{return !is_empty() || _closed;});

        status result = status::success;
        if(_closed)
        {
          result = status::closed;
        }
        else
        {
          x = _queue.front();
          _queue.pop();
        }
        lock.unlock();
        _cv.notify_one();
        return result;
      }
      // Closes the queue.
      // The queue is placed in the closed state.
      // The closed state prevents more items from being inserted
      // on the queue, but it does not clear the items that are
      // already on the queue.
      // Invoking this function on a closed queue has no effect.
      // This function is thread safe.
      void close ()
      {
        std::unique_lock lock(_queue_mutex, std::defer_lock);
        _cv.wait(lock);
        _closed = true;
        lock.unlock();
        _cv.notify_one();
      }
      
      // Clears the queue.
      // All of the elements on the queue are discarded.
      // This function is thread safe.
      //TODO: MAKE IT THREAD SAFE
      void clear ()
      {
        std::unique_lock lock(_queue_mutex, std::defer_lock);
        _cv.wait(lock);
        while(!is_empty())
        {
         _queue.pop(); 
        }
        lock.unlock();
        _cv.notify_one();
      }
      
      // Returns if the queue is currently full (i.e., the number of
      // elements in the queue equals the maximum queue size).
      // This function is not thread safe.
      bool is_full () const
      {
        return _queue.size() == _max_size;
      }
      
      // Returns if the queue is currently empty.
      // This function is not thread safe.
      bool is_empty () const
      {
        return _queue.empty();
      }
      
      // Returns if the queue is closed (i.e., in the closed state).
      // This function is not thread safe.
      bool is_closed () const
      {
        return _closed;
      }
      
      // Returns the maximum number of elements that can be held in
      // the queue.
      // This function is not thread safe.
      size_type max_size () const
      {
        return _max_size;
      }
      
    private:
      size_t _max_size;
      std::queue<T> _queue;
      bool _closed; 
      mutable std::mutex _queue_mutex;
      // mutable std::condition_variable _e; //not empty or full
      // mutable std::condition_variable _f; //not full
      mutable std::condition_variable _cv; //not full

  };
  }
