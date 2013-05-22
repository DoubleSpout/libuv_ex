global.n = 5;
global.cb = function(str){
	console.log(str||'')
	if(!--global.n){
		console.log('all test down');
		process.exit(0);
	}
}


require('./sync.js')
setTimeout(function(){
	require('./sync_thread.js')
},1000*15);
setTimeout(function(){
	require('./asyn.js')	
},1000*15*2);
setTimeout(function(){
	require('./asyn_thread.js')
},1000*15*3);
setTimeout(function(){
	require('./asyn_pool.js')
},1000*15*5);
