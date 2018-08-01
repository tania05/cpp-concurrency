#include "ra/queue.hpp"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <cassert>

template<class T>
void test_constructor()
{
    ra::concurrency::queue<T> q(6);

    std::cout << "Made a queue" << std::endl;
    assert(q.max_size() == 6);
    assert(!q.is_closed());
    assert(!q.is_full());
    assert(q.is_empty());

    std::thread t1(q.push(9));
    t1.join();
    std::cout << "done" << std::endl;

}

int main()
{
    test_constructor<int>();
    return 0;
}
