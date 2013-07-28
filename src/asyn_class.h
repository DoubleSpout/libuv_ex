#ifndef ASYN_H
#define ASYN_H
#include <node.h>
#include <string>


using namespace v8;

class Asyn {

 public:
  static Handle<Value> do_asyn(const Arguments& args);
  static Handle<Value> do_asyn_thread(const Arguments& args);
  static Handle<Value> do_asyn_pool(const Arguments& args);
  static Handle<Value> do_sync(const Arguments& args);
  static Handle<Value> do_sync_thread(const Arguments& args);

  
 private:
  static void thread_callback(void* arg);//sync_thread used
  static void asyn_callback(uv_async_t* handle, int status);//asyn used
  static void asyn_thread_work(void* arg);//asyn_thread used
  static void asyn_thread_callback(uv_async_t* handle, int status);//asyn_thread used
  static void asyn_pool_callback(uv_work_t* req);//asyn_pool used
  static void asyn_after_pool_callback(uv_work_t *req, int status);//asyn_pool used
  static void uv_close_func(uv_handle_t* handle);//close the uv and delete the pointer
  Asyn();
  ~Asyn();
  
};

#endif