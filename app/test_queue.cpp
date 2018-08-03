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

template<class T>
void test_more()
{
    ra::concurrency::queue<T> q(4);
    std::cout << "Made a queue" << std::endl;
    assert(q.max_size() == 4);
    assert(!q.is_closed());
    assert(!q.is_full());
    assert(q.is_empty());

    T result;

    std::thread t1(&ra::concurrency::queue<T>::push, &q, 9);
    std::thread t2(&ra::concurrency::queue<T>::push, &q, 6);
    std::thread t4(&ra::concurrency::queue<T>::push, &q, 2);
    std::thread t6(&ra::concurrency::queue<T>::push, &q, 3);
    std::thread t7(&ra::concurrency::queue<T>::push, &q, 6);
    std::thread t8(&ra::concurrency::queue<T>::push, &q, 7);
    std::thread t9(&ra::concurrency::queue<T>::push, &q, 8);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t3(&ra::concurrency::queue<T>::pop, &q, std::ref(result));
    std::thread t5(&ra::concurrency::queue<T>::pop, &q, std::ref(result));
    std::thread tB(&ra::concurrency::queue<T>::pop, &q, std::ref(result));
    std::thread tA(&ra::concurrency::queue<T>::close, &q);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    tA.join();
    tB.join();

    std::cout << "Done" << std::endl;
}


int main()
{
    test_constructor<int>();
    ra::concurrency::queue<int> q(19);
    std::thread t1(checkRandom<int>, std::ref(q));
    t1.join();
    test_more<float>();
    return 0;
}
