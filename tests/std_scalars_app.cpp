
#include "std_scalars-rpc.hpp"
#ifdef SimpleRPC
using namespace simple_rpc::std_scalars;
#endif

#include <iostream>
#include <typeinfo> 
#include <sys/time.h>

static unsigned int check_getarg_success_count = 0;
static unsigned int check_getarg_failure_count = 0;

template <typename T> bool check_getarg(T a)
{
  T r = getarg(a);
  bool success = (r == a);
  if (!success)
    {
      std::cout << "check_getarg failed: getarg<"<<typeid(T).name()<<">("<<a<<")->"<<r<<" but expected "<<a<<std::endl;
      check_getarg_failure_count ++;
    }
  else
    check_getarg_success_count ++;
  return success;
}

template <typename T> bool check_getarg_container(T& a)
{
  T r = getarg(a);
  bool success = (r == a);
  if (!success)
    {
      std::cout << "check_getarg failed: getarg<"<<typeid(T).name()<<">(["<<a.size()<<"])->["<<r.size()<<"] but expected given argument"<<std::endl;
      check_getarg_failure_count ++;
    }
  else
    check_getarg_success_count ++;
  return success;
}

main()
{
  SIMPLE_RPC_CONNECT("127.0.0.1", 2340, 0);

  check_getarg((bool)3);
  check_getarg((char)3);
  check_getarg((short)3);
  check_getarg((long)3);
  check_getarg((char)3);
  check_getarg((float)3.14);
  check_getarg((double)3.14);
  check_getarg(std::string("Hello"));
  {
    std::vector<char> v(3); v[0] = 1; v[1] = 2; v[2] = 3;
    check_getarg_container(v);
  }
  {
    std::vector<short> v(3); v[0] = 1; v[1] = 2; v[2] = 3;
    check_getarg_container(v);
  }
  {
    std::vector<int> v(3); v[0] = 1; v[1] = 2; v[2] = 3;
    check_getarg_container(v);
  }
  {
    std::vector<long> v(3); v[0] = 1; v[1] = 2; v[2] = 3;
    check_getarg_container(v);
  }
  {
    std::vector<float> v(3); v[0] = 1.1; v[1] = 2.2; v[2] = 3.3;
    check_getarg_container(v);
  }
  {
    std::vector<double> v(3); v[0] = 1.1; v[1] = 2.2; v[2] = 3.3;
    check_getarg_container(v);
  }
  {
    std::vector<std::string> v(3); v[0] = "hello"; v[1] = "hey"; v[2] = "hi";
    check_getarg_container(v);
  }
  {
    std::list<int> v; v.push_back(1); v.push_back(2); v.push_back(3); 
    check_getarg_container(v);
  }

  //
  std::cout<<"check_getarg "<<check_getarg_success_count<<" tests PASSED OK"<<std::endl;
  if (check_getarg_failure_count)
    std::cout<<"check_getarg "<<check_getarg_failure_count<<" tests FAILED"<<std::endl;

#define TIMEIT(ELAPSED, BODY) \
  {\
    struct timeval start_time;\
    struct timeval end_time;\
    gettimeofday(&start_time, NULL);\
    BODY \
    gettimeofday(&end_time, NULL);\
    double start = start_time.tv_sec*1000000 + (start_time.tv_usec); \
    double end   = end_time.tv_sec*1000000 + (end_time.tv_usec); \
    ELAPSED     = (end - start); \
  }

  {
    double elapsed;
    int count = 100;
    std::vector<double> v(300); v[0] = 1.1;// v[1] = 2.2; v[2] = 3.3;

    TIMEIT(elapsed,
    for (int i=0; i<count; i++)
      {
	getarg(v);
      }
	   )
    std::cout << "getarg<vector<double>> timing=" <<elapsed/count<<"us per call"<<std::endl;


    {
      std::string value("12345");
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  getarg(value);
	}
	     )
    }
    std::cout << "getarg<string> timing=" <<elapsed/count<<"us per call"<<std::endl;


    {
      std::string value("12345");
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  passarg(value);
	}
	   )
    }
    std::cout << "passarg<string> timing=" <<elapsed/count<<"us per call"<<std::endl;

    {
      double value = 3.14;
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  getarg(value);
	}
	     )
    }
    std::cout << "getarg<double> timing=" <<elapsed/count<<"us per call"<<std::endl;

    {
      int value = 3.14;
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  getarg(value);
	}
	     )
    }
    std::cout << "getarg<int> timing=" <<elapsed/count<<"us per call"<<std::endl;
  }
  {
    int count = 100;
    double elapsed;
    {
      int value = 3.14;
      std::vector<double> a(3); 
      a[0] = 1.1; a[1] = 2.2; a[2] = 3.3;
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  sum(a);
	}
	     )
    }
    std::cout << "sum(a) timing=" <<elapsed/count<<"us per call"<<std::endl;

    {
      int value = 3.14;
      std::vector<double> a(3); 
      a[0] = 1.1; a[1] = 2.2; a[2] = 3.3;
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  sum(a,a);
	}
	     )
    }
    std::cout << "sum(a,a) timing=" <<elapsed/count<<"us per call"<<std::endl;

    {
      int value = 3.14;
      std::vector<double> a(3); 
      a[0] = 1.1; a[1] = 2.2; a[2] = 3.3;
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  sum(a,a,a,a,a,a);
	}
	     )
    }
    std::cout << "sum(a,a,a,a,a,a) timing=" <<elapsed/count<<"us per call"<<std::endl;

    {
      int value = 3.14;
      std::vector<double> a(3), b(3,0); 
      a[0] = 1.1; a[1] = 2.2; a[2] = 3.3;
      swap(a,b);
      if (!(a[0] == 0 && b[0] == 1.1))
	{
	  std::cout << "swap failed" << std::endl;
	  return 0;
	}
      TIMEIT(elapsed,
      for (int i=0; i<count; i++)
	{
	  swap(a,b);
	}
	     )
    }
    
    std::cout << "swap(a,b) timing=" <<elapsed/count<<"us per call"<<std::endl;

  }
  return check_getarg_failure_count;
}
