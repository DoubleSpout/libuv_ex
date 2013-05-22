var asyn_obj = require('../build/Release/asyn.node');

var check = function(t, n, func){
	if(!isFinite(t)) throw("arg[0] must a number")
	if(!isFinite(n)) throw("arg[1] must a number")
	if("function" !== typeof func) throw("arg[2] must a function")
	return true;
}

var asyn = function(t,i,f){
	check(t,i,f) && asyn_obj.asyn(t,i,f);
}
var asyn_thread = function(t,i,f){
	check(t,i,f) && asyn_obj.asyn_thread(t,i,f);
}
var asyn_pool = function(t,i,f){
	check(t,i,f) && asyn_obj.asyn_pool(t,i,f);
}
var sync = function(t,i,f){
	check(t,i,f) && asyn_obj.sync(t,i,f);
}
var sync_thread = function(t,i,f){
	check(t,i,f) && asyn_obj.sync_thread(t,i,f);
}


module.exports = {
	asyn:asyn,
	asyn_thread:asyn_thread,
    asyn_pool:asyn_pool,
	sync:sync,
	sync_thread:sync_thread
};
