namespace ra :: concurrency {
// Thread pool class.
class thread_pool
{
  public:
  // An unsigned integral type used to represent sizes.
  using size_type = std :: size_t ;
  // Creates a thread pool with the number of threads equal to the
  // hardware concurrency level (if known); otherwise the number of
  // threads is set to 2.
  thread_pool ();
  // Creates a thread pool with num_threads threads.
  // Precondition: num_threads > 0
  thread_pool ( std :: size_t num_threads );
  // A thread pool is not copyable or movable.
  thread_pool (const thread_pool &) = delete;
  thread_pool & operator=(const thread_pool &) = delete;
  thread_pool ( thread_pool &&) = delete;
  thread_pool & operator=( thread_pool &&) = delete;
  // Destroys a thread pool, shutting down the thread pool first
  // (if not already shutdown).
  ˜ thread_pool ();
  // Gets the number of threads in the thread pool.
  // This function is not thread safe.
  size_type size () const;
  // Enqueues a task for execution by the thread pool.
  // The task to be performed is the code associated with the
  // callable entity func.
  // Note: The rvalue reference parameter is intentional and
  // implies that the schedule function is permitted to change
  // the value of func (e.g., by moving from func).
  // Precondition: The thread pool is not in the shutdown state.
  // This function is thread safe.
  void schedule ( std :: function <void() >&& func );
  // Shuts down the thread pool.
  // This function does not return until all threads have
  // completed their tasks.
  // If the thread pool is already shutdown, this function has
  // no effect.
  // After the thread pool is shutdown, it can only be destroyed.
  // This function is thread safe.
  void shutdown ();
  // Tests if the thread pool has been shutdown.
  // This function is not thread safe.
  bool is_shutdown () const;
};
}
