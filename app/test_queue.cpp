#include "ra/queue.hpp"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <chrono>



template<class T>
void test_constructor()
{
    ra::concurrency::queue<T> q(6);

    std::cout << "Made a queue" << std::endl;
    assert(q.max_size() == 6);
    std::cout << "Made1" << std::endl;
    assert(!q.is_closed());
    std::cout << "Made2" << std::endl;
    assert(!q.is_full());
    std::cout << "Made3" << std::endl;
    assert(q.is_empty());

    std::thread t1(&ra::concurrency::queue<T>::push, &q, 9);
    
    t1.join();
    // assert(!q.is_empty());    
    std::cout << "done" << std::endl;

    // exit(0);
}

template<class T>
void checkRandom(ra::concurrency::queue<T>& q)
{
    std::cout << "Starting random call" << std::endl;
    for(std::size_t i =0; i< std::rand() % 10; ++i)
    {
        std::cout << "pushing" << T(i) << std::endl;
        assert(q.push(T(i)) == ra::concurrency::queue<T>::status::success);
    }
    assert(!q.is_empty());
    std::cout << "Done Pushing" << std::endl;
}


int main()
{
    test_constructor<int>();
    ra::concurrency::queue<int> q(19);
    std::thread t1(checkRandom<int>, std::ref(q));
    t1.join();
    return 0;
}
