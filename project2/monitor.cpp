#include "monitor.h"

Monitor::Monitor(int numThreads, int numRecords){

	records = (Record **)malloc(sizeof(Record*));
	for(int i = 0; i < numRecords; i++){
		records[i] = new Record(i);
	}

	threads_info.resize(numThreads, vector<int>(3, UNDEFINED));
}


Monitor::~Monitor(){

}

int Monitor::get(LockInfo req){
	if(req.type == LockType::R){
		rwlock.lockShared(index, monitor, req);
	} else {
		rwlock.lockExclusive(index, monitor, req);
	}
}


void Monitor::set(int _value){

}


void Monitor::release(LockInfo req){

}


bool Monitor::deadlock_check(int record, LockInfo request){
	lock_request[record].push(request);
	return true;
}

LockInfo Monitor::getFirstReq(int record){
	return lock_request[record].front();
}