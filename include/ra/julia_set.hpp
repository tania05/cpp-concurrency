#include <complex>
#include "boost/multi_array.hpp"
#include <chrono>
#include <string>
#include <iostream>
#include "ra/thread_pool.hpp"

namespace ra :: fractal {


template <class Real>
std::complex<Real>  f_z(const std::complex <Real> z, const std::complex <Real>& c)
{
  // auto k = std::pow(z,2) + c;
  // std::cout << "In fz : " << "re: " << k.real() << " img: " << k.imag() << std::endl; 
  return (z*z) + c;
}

template <class Real>
std::complex<Real>  z_n(int n, std::complex<Real> z, const std::complex<Real>& c)
{

  if(n == 0)
  {
    return z;
  }
  z = f_z(z_n(n-1,z,c), c);

  return z;
}


template <class Real>
int n_z(int m, std::complex <Real> z, const std::complex <Real>& c)
{
  for(int i=0; i< m; ++i)
  {
    // auto ch = z_n(i, z, c);
    // std::cout << "VALUE GOTTEN BACK AT n_z " << ch.real() << " " << ch.imag() << " " << "i: " << i << std::endl;
    // std::cout << std::abs(ch) << std::endl;  
    if(std::abs(z_n(i, z, c)) > (Real) 2)
    {
      return i;
    }
  }
  //did not find i that gives 2, so return the max we can get
  return std::numeric_limits<int>::max();
}


template <class Real>
int  y_m(int m, std::complex <Real> z, const std::complex <Real>& c)
{
  return std::min(n_z(m, z, c), m);
}


template <class Real>
std::complex<Real> get_z(Real u_0, Real u_1, Real v_0, Real v_1, int l, int k, int H, int W)
{
  // std::cout << "Insize get z" << std::endl;
  // std::cout << l << std::endl;
  // std::cout << k << std::endl;
  Real re = u_0 + ((Real)k/(Real)(W-1))*(v_0 - u_0);
  Real im = u_1 + ((Real)l/(Real)(H-1))*(v_1 - u_1);
  
  // Real re = k;
  // Real im = l;
  
  std::complex <Real> r(re,im);
  // std::cout << u_0 << " ";
  // std::cout << u_1 << " ";
  // std::cout << v_0 << " ";
  // std::cout << v_1 << " ";
  // std::cout << l << " ";
  // std::cout << k << " ";
  // std::cout << H << " ";
  // std::cout << W << " ";
  // std::cout << re << " ";
  // std::cout << im << std::endl;
  return r;
}

  template <class Real>
  void compute_julia_set (const std::complex <Real>& bottom_left ,
                          const std::complex <Real>& top_right,
                          const std::complex <Real>& c ,
                          int max_iters , //m
                          boost::multi_array <int, 2>& a,
                          int num_threads )
  {
    ra::concurrency::thread_pool tp2(num_threads);
    std::mutex  mcout;
    int H = a.shape()[0];
    int W = a.shape()[1];
    //get the row -> height H
    for (int i = H-1; i >= 0; --i)
    {
      // std::cout << i << std::endl;
      tp2.schedule([i, &mcout, bottom_left, top_right, H, W, max_iters, c, &a]() {
        // mcout.lock();
        //for each column in the row -> width H
        for (int k = 0; k < W; ++k)
        {
          //get z for each point
          std::complex <Real> z = get_z(bottom_left.real(), 
                                              bottom_left.imag(), 
                                              top_right.real(), 
                                              top_right.imag(),
                                              i,
                                              k,
                                              H,
                                              W
                                              );

          a[i][k] = y_m(max_iters,z,c);
        }
        // std::cout << "dd" << std::endl;
        mcout.unlock();
      });
    }

    tp2.shutdown();
  }

}