#include "monitor.h"

Monitor::Monitor(int numThreads, int numRecords){
	//initiaiize 
	records = new Record[numRecords];
	for(int i = 0; i < numRecords; i++){
		records[i].setIndex(i);
	}

	//set memory 
	threads_info.resize(numThreads, vector<int>(3, UNDEFINED));
	lock_request.resize(numRecords, vector<LockInfo>());
	dependThreads.resize(numThreads, set<int>());
}


Monitor::~Monitor(){

}

//getLock and set flag
void Monitor::getLock(LockInfo req){
	if(req.type == LockType::R){
		records[req.index].getReaderLock(lock_request[req.index],req);
		for (LockInfo& r : lock_request[req.index]) {
			if (r == req)
				r.getLock = true;
		}
	} else {
		records[req.index].getWriterLock(lock_request[req.index],req);
		for (LockInfo& r : lock_request[req.index]) {
			if (r == req)
				r.getLock = true;
		}
	}
}

//delete lock from queue
void Monitor::deleteLock(LockInfo req){
	vector<LockInfo>::iterator it;

  	it = find (lock_request[req.index].begin(), lock_request[req.index].end(), req);
  	if(it != lock_request[req.index].end())
  		lock_request[req.index].erase(it);

  	//change thread info to default
	threads_info[req.tid][req.type] = UNDEFINED;

	//change dependency graph to default
	dependThreads[req.tid].clear();
	for (set<int>& s : dependThreads) {
		s.erase(req.tid);
	}
}

void Monitor::releaseLock(LockInfo req){
	//delete lock info from queue
	deleteLock(req);
	
	if(req.type == LockType::R){
		records[req.index].releaseReaderLock();
	} else {
		records[req.index].releaseWriterLock();
	}
}

Status Monitor::deadlock_check(LockInfo req) {

	LockInfo cur;
	set<int> visited;

	//add dependency
	for (LockInfo r : lock_request[req.index]) {
		if (req.type == LockType::R && r.type != LockType::R) {
			dependThreads[req.tid].insert(r.tid);
		}
		if (req.type != LockType::R) {
			dependThreads[req.tid].insert(r.tid);
		}
	}

	//find cycle
	queue<int> checkDepend;
	for (int tid : dependThreads[req.tid]) {
		//push depend thread into queue
		checkDepend.push(tid);
	}

	while (!checkDepend.empty()) {
		int cur = checkDepend.front();
		checkDepend.pop();

		//if find cycle, return deadlock
		if (cur == req.tid)
			return Status::deadlock;
		if (visited.find(cur) != visited.end())
			continue;

		//add dependency of current thread
		for (int tid : dependThreads[cur])
			checkDepend.push(tid);
		visited.insert(cur);
	}

	lock_request[req.index].push_back(req);
	threads_info[req.tid][req.type] = req.index;

	//first of lock waiting queue => get lock
	if (lock_request[req.index].front() == req)
		return Status::get;

	//if request reader lock and there is no writer
	//get reader lock
	if (req.type == LockType::R) {
		for (LockInfo r : lock_request[req.index]) {
			if (r == req) {
				break;
			}
			if (r.type != LockType::R)
				return Status::waiting;
		}
		return Status::get;
	}
	
	//else waiting
	return Status::waiting;
}

//read record value
int Monitor::read(int index) {
	return records[index].read();
}

//write to record as value
void Monitor::write(int index, int value) {
	records[index].set(value);
}
