console.time("asyn_thread_all_the_work")
console.time("main_thread_js_work")
var assert = require('assert');
var asyn = require('../index.js')



//do fibo 10 times
var i = 40;
var times = 10;
var job_times = 11;

asyn.asyn_thread(times,i,function(err, result){
	console.log('fibo('+i+'):' + result)
	console.log('c++ thread asyn thread callback')
		assert.strictEqual(result, 102334155)
	if(!--job_times){
		console.timeEnd("asyn_thread_all_the_work")
		global.cb();
	}
})



var d1 = Date.now();
console.log('main thread start do js work');
while(Date.now() - d1 <= 1000*1){
	//sleeping 10sec simulate the js work
}
console.log('main thread end to do js work');
console.timeEnd("main_thread_js_work")



if(!--job_times){
	console.timeEnd("asyn_thread_all_the_work")
	global.cb();
}



