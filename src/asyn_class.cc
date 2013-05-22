#include <node.h>
#include <string>
#include <iostream>


#include "asyn_class.h"
#include "job_class.h"

uv_rwlock_t numlock;
  
using namespace v8;


//js code asyn function
Handle<Value> Asyn::do_asyn(const Arguments& args) {

  HandleScope scope;

  int times = args[0]->Int32Value();

  int times2 = times;

  int fibo_num = args[1]->Int32Value();
  
  static int count = 0;

  //std::cout<<times<<std::endl;



  while(times--){
	Job *job_ptr = new Job();
	
	//std::cout<<job_ptr<<std::endl;

	job_ptr->js_obj = Persistent<Object>::New(args[2]->ToObject());
	 
	job_ptr->fibo_num = fibo_num;

	job_ptr->times = times2;

	job_ptr->count = &count;

	job_ptr->main_async.data = job_ptr;
	//regist async callback
	uv_async_init(uv_default_loop(), &job_ptr->main_async, asyn_callback);	
	//put the data in to asyn callback parameter

	//emit the async
	uv_async_send(&job_ptr->main_async);
 }

  return scope.Close(Undefined());
}


Handle<Value> Asyn::do_asyn_thread(const Arguments& args) {
  HandleScope scope;

  int times = args[0]->Int32Value();

  int times2 = times;

  int fibo_num = args[1]->Int32Value();
  
  static int count = 0;


  while(times--){

	//std::cout<<times<<std::endl;

	Job *job_ptr = new Job();

	job_ptr->js_obj = Persistent<Object>::New(args[2]->ToObject());
	  
	job_ptr->fibo_num = fibo_num;

	job_ptr->times = times2;

	job_ptr->count = &count;

	//regist async callback
	uv_async_init(uv_default_loop(), &job_ptr->main_async, asyn_thread_callback);
	
	//create thread
	uv_thread_create(&job_ptr->worker_thread, asyn_thread_work, job_ptr);
	//using the thread to do the fibo job
	//uv_thread_join(&job_ptr->worker_thread);

 }

  return scope.Close(Undefined());  
}


Handle<Value> Asyn::do_asyn_pool(const Arguments& args) {
  HandleScope scope;

  int times = args[0]->Int32Value();

  int times2 = times;

  int fibo_num = args[1]->Int32Value();

  static int count = 0;

  while(times--){

	//std::cout<<times<<std::endl;

	Job *job_ptr = new Job();
	
	job_ptr->js_obj = Persistent<Object>::New(args[2]->ToObject());
	  
	job_ptr->fibo_num = fibo_num;

	job_ptr->times = times2;

	job_ptr->count = &count;
	
	job_ptr->work_pool.data = job_ptr;

	uv_queue_work(uv_default_loop(), &job_ptr->work_pool, asyn_pool_callback, asyn_after_pool_callback);
	
 }

  return scope.Close(Undefined());  
}





Handle<Value> Asyn::do_sync(const Arguments& args) {

  HandleScope scope;
 
  Job *job_ptr = new Job();
  
  job_ptr->js_obj = Persistent<Object>::New(args[2]->ToObject());
  
  job_ptr->fibo_num = args[1]->Int32Value();

  int times = args[0]->Int32Value();

  //do fibo work normal
  while(times--){

	  job_ptr->fibo_res = Job::fibo(job_ptr->fibo_num);
	  
	  Local<Value> argv[2];
	  argv[0] = Local<Value>::New(Null());
	  argv[1] = Number::New(job_ptr->fibo_res);
	 
	  job_ptr->js_obj->CallAsFunction(Object::New(), 2, argv);
  
  }


  job_ptr->js_obj.Dispose();
  delete job_ptr;

  return scope.Close(Undefined());
}




Handle<Value> Asyn::do_sync_thread(const Arguments& args) {

  HandleScope scope;
 
  Job *job_ptr = new Job();
  
  job_ptr->js_obj = Persistent<Object>::New(args[2]->ToObject());
  
  job_ptr->fibo_num = args[1]->Int32Value();

  int times = args[0]->Int32Value();

  int times2 = times;

  //do multi thread Synchronous
  while(times--){
	  //create thread
	  uv_thread_create(&job_ptr->worker_thread, thread_callback, job_ptr);
	  //using the thread to do the fibo job
	  uv_thread_join(&job_ptr->worker_thread); 

	  
	  Local<Value> argv[2];
	  argv[0] = Local<Value>::New(Null());
	  argv[1] = Number::New(job_ptr->fibo_res);

	  job_ptr->js_obj->CallAsFunction(Object::New(), 2, argv);

  }
  


  job_ptr->js_obj.Dispose();

  delete job_ptr;

  return scope.Close(Undefined());

}





//thread callback job
void Asyn::thread_callback(void* arg){ 

  Job *job_ptr = (Job *) arg;

  job_ptr->fibo_res =  Job::fibo(job_ptr->fibo_num);

  //put the data in to asyn callback parameter
  job_ptr->main_async.data = job_ptr;
	
  //emit the async
  uv_async_send(&job_ptr->main_async);

}






//async callback
void Asyn::asyn_callback(uv_async_t* handle, int status){
	
	HandleScope scope;

	Job* job_ptr= (Job *) handle->data;
	
	//std::cout<<job_ptr->fibo_num<<std::endl;

	job_ptr->fibo_res = Job::fibo(job_ptr->fibo_num);
	
	Local<Value> argv[2];
	argv[0] = Local<Value>::New(Null());
	argv[1] = Number::New(job_ptr->fibo_res);

	job_ptr->js_obj->CallAsFunction(Object::New(), 2, argv);
	
	job_ptr->js_obj.Dispose();

	uv_rwlock_wrlock(&numlock);

	(*job_ptr->count)++;

	uv_rwlock_wrunlock(&numlock);
	
	//std::cout<<job_ptr<<std::endl;

	uv_close((uv_handle_t*) &job_ptr->main_async, uv_close_func);


    scope.Close(Undefined());

}

void Asyn::asyn_thread_callback(uv_async_t* handle, int status){

	HandleScope scope;

	Job* job_ptr= (Job *) handle->data;

	Local<Value> argv[2];
	argv[0] = Local<Value>::New(Null());
	argv[1] = Number::New(job_ptr->fibo_res);

	
	job_ptr->js_obj->CallAsFunction(Object::New(), 2, argv);
	
	job_ptr->js_obj.Dispose();

	uv_rwlock_wrlock(&numlock);

	(*job_ptr->count)++;

	uv_rwlock_wrunlock(&numlock);
	
	//std::cout<<job_ptr<<std::endl;

	uv_close((uv_handle_t*) &job_ptr->main_async, uv_close_func);


    scope.Close(Undefined());

	

}


//thread callback do fibo
void Asyn::asyn_thread_work(void* arg){
	
	Job *job_ptr = (Job *) arg;

	job_ptr->fibo_res = Job::fibo(job_ptr->fibo_num);
	
	job_ptr->main_async.data = job_ptr;

	uv_async_send(&job_ptr->main_async);

}






//async pool callback
void Asyn::asyn_pool_callback(uv_work_t* req){
	
	Job* job_ptr= (Job *) req->data;

	job_ptr->fibo_res = Job::fibo(job_ptr->fibo_num);

}

//async after pool callback
void Asyn::asyn_after_pool_callback(uv_work_t *req, int status){
	HandleScope scope;
	
	Job* job_ptr= (Job *) req->data;

	Local<Value> argv[2];
	argv[0] = Local<Value>::New(Null());
	argv[1] = Number::New(job_ptr->fibo_res);

	job_ptr->js_obj->CallAsFunction(Object::New(), 2, argv);
	
	job_ptr->js_obj.Dispose();
	
	
	uv_rwlock_wrlock(&numlock);
	(*job_ptr->count)++;
	uv_rwlock_wrunlock(&numlock);

	//std::cout<<(*job_ptr->count)<<std::endl;

	//std::cout<<job_ptr->times<<std::endl;
	
	//uv_close((uv_handle_t*) &job_ptr->work_pool, uv_close_func);
	
	delete job_ptr;

    scope.Close(Undefined());

}

void Asyn::uv_close_func(uv_handle_t* handle){
	
	Job* job_ptr= (Job *) handle->data;

	delete job_ptr;
	
};


std::string Asyn::toCString(Handle<Value> strp){
      String::Utf8Value utf8_value(strp->ToString());//conver to utf8-value
      std::string str = *utf8_value;
      return str;
}


Asyn::Asyn(){};
Asyn::~Asyn(){};