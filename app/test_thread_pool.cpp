#include "ra/thread_pool.hpp"

int main()
{
  std::cout << "HERE" << std::endl;

  ra::concurrency::thread_pool tp(6);
  std::mutex  mcout;
  tp.shutdown();
  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 1" << std::endl;
    }
  });
  
  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 2" << std::endl;
    }
  });

  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 3" << std::endl;
    }
  });

  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 4" << std::endl;
    }
  });

  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 5" << std::endl;
    }
  });
  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 6" << std::endl;
    }
  });
  
  tp.schedule([&mcout]() 
  {
    for (int i=0; i< 10; ++i)
    {
      std::unique_lock<std::mutex> lock(mcout);
      std::cout << "Ran Task 7" << std::endl;
    }
  });

  tp.shutdown();
  

  ra::concurrency::thread_pool tp2(6);
	for (int i = 0; i < 32; ++i)
	{
		tp2.schedule([i, &mcout]() {
			for (int j = 0; j < 100; ++j);
      mcout.lock();
			std::cout << "Ran Task " << i << std::endl;
      mcout.unlock();
		});
	}
	
	tp2.shutdown();
  std::cout.flush();

  return 0;
}