#include "ra/julia_set.hpp"
#include <string>
#include <complex>
#include "boost/multi_array.hpp"
#include <chrono>
#include <iostream>

std::string make_plot(boost::multi_array<int, 2> a)
{
      std::string str;
      std::size_t H = a.shape()[0];
      std::size_t W = a.shape()[1];
    
      str.push_back('P');
      str.push_back('2');
      str.append(" ");
      str.append(std::to_string(W));
      str.append(" ");
      str.append(std::to_string(H));
      str.append(" ");
      str.append(std::to_string(255));
      str.push_back('\n');
      

      for(std::size_t i = 0; i< H; ++i)
      {
          for(std::size_t j = 0 ; j < W; ++j)
          {
              str.append(std::to_string( a[i][j] ));
              str.append(" ");              
          }
          str.push_back('\n');
      }
      return str;
}


int main()
{

  std::complex<float> bottom_left(-1.25, -1.25);
  std::complex<float> top_right(1.25, 1.25);
  boost::multi_array <int, 2> a(boost::extents [512][512] );
  int max_iters = 255;
  std::complex<float> c(0.37, -0.16);
  
  ra::fractal::compute_julia_set (bottom_left, top_right, c, max_iters, a, 1);
  
  std::cout << make_plot(a);
  // auto z = std::complex<float>(0,0);
  // auto c = std::complex<float>(1,1);
  
  // std::cout << ra::fractal::y_m<float>(255, z, c) << std::endl;
  return 0;
}