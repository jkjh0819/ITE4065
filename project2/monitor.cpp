#include "monitor.h"

Monitor::Monitor(int numThreads, int numRecords){

	records = (Record **)malloc(sizeof(Record*));
	for(int i = 0; i < numRecords; i++){
		records[i] = new Record(i);
	}

	threads_info.resize(numThreads, vector<int>(3, UNDEFINED));
	lock_request.resize(numRecords, queue<LockInfo>());
}


Monitor::~Monitor(){

}

int Monitor::get(LockInfo req){
	if(req.type == LockType::R){
		records[req.index]->readerLock(lock_request[req.index],req);
	} else {
		records[req.index]->writerLock(lock_request[req.index],req);
	}
}


void Monitor::set(int _value){

}


void Monitor::release(LockInfo req){

}


bool Monitor::deadlock_check(int record, LockInfo request){
	cout << "begin deadlock_check" << endl;
	lock_request[record].push(request);
	cout << "end deadlock_check" << endl;
	return false;
}

LockInfo Monitor::getFirstReq(int record){
	return lock_request[record].front();
}