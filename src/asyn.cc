
#include <node.h>
#include "asyn_class.h"



using namespace v8;

void Init(Handle<Object> target) {

  HandleScope scope;

  target->Set(String::NewSymbol("asyn"),
           FunctionTemplate::New(Asyn::do_asyn)->GetFunction());

  target->Set(String::NewSymbol("asyn_thread"),
           FunctionTemplate::New(Asyn::do_asyn_thread)->GetFunction());

  target->Set(String::NewSymbol("asyn_pool"),
           FunctionTemplate::New(Asyn::do_asyn_pool)->GetFunction());

  target->Set(String::NewSymbol("sync"),
           FunctionTemplate::New(Asyn::do_sync)->GetFunction());

  target->Set(String::NewSymbol("sync_thread"),
           FunctionTemplate::New(Asyn::do_sync_thread)->GetFunction());


}

NODE_MODULE(asyn, Init)