#ifndef JOB_H
#define JOB_H
#include <node.h>
#include <string>


using namespace v8;

class Job {

  public:

    Persistent<Object> js_obj;

	uv_async_t main_async;

	uv_work_t work_pool;

	uv_loop_t* loop;

    uv_thread_t worker_thread;

	int *count;
    
	int fibo_num;

	int times;

	double fibo_res;

    static int fibo(int n);

    Job();

    ~Job();
  
};

#endif