#include <node.h>
#include <string>
#include <iostream>


#include "job_class.h"

  
using namespace v8;


//fibo function
int Job::fibo(int n){

  if(1 == n || 2 == n){
      return 1;
  }
  else{
      return fibo(n-1) + fibo(n-2);
  }

  return 0;
}



Job::Job(){};
Job::~Job(){};












