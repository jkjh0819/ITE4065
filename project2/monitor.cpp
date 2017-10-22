#include "monitor.h"

Monitor::Monitor(int numThreads, int numRecords){

	//records = (Record *)malloc(sizeof(Record*));
	records = new Record[numRecords];
	for(int i = 0; i < numRecords; i++){
		records[i].setIndex(i);
		//cout << records[i]->getIndex() << endl;
	}
	//cout << records[0].getIndex() << endl;

	threads_info.resize(numThreads, vector<int>(3, UNDEFINED));
	lock_request.resize(numRecords, vector<LockInfo>());
	dependThreads.resize(numThreads, set<int>());
}


Monitor::~Monitor(){

}

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

void Monitor::cancelLock(LockInfo req) {
	vector<LockInfo>::iterator it;

	it = find(lock_request[req.index].begin(), lock_request[req.index].end(), req);
	if (it != lock_request[req.index].end())
		lock_request[req.index].erase(it);

	threads_info[req.tid][req.type] = UNDEFINED;
	records[req.index].noticeCancel();

	dependThreads[req.tid].clear();
	for (set<int>& s : dependThreads) {
		s.erase(req.tid);
	}
}

void Monitor::deleteLock(LockInfo req){
	vector<LockInfo>::iterator it;

  	it = find (lock_request[req.index].begin(), lock_request[req.index].end(), req);
  	if(it != lock_request[req.index].end())
  		lock_request[req.index].erase(it);

	threads_info[req.tid][req.type] = UNDEFINED;

	dependThreads[req.tid].clear();
	for (set<int>& s : dependThreads) {
		s.erase(req.tid);
	}
}

void Monitor::releaseLock(LockInfo req){
	//cout << records[req.index]->getIndex() << endl;
	deleteLock(req);
	
	if(req.type == LockType::R){
		records[req.index].releaseReaderLock();
	} else {
		records[req.index].releaseWriterLock();
	}


	//cout << "after release" << endl;
	//printRecordQueue();
}

Status Monitor::deadlock_check(LockInfo req) {
	
		printRecordQueue();
	cout << ">> request " << req.tid << "-" << req.type << " to " << req.index << endl;


	LockInfo cur;
	stack<LockInfo> wait_for;
	set<int> visited;

	for (LockInfo r : lock_request[req.index]) {
		if (req.type == LockType::R && r.type != LockType::R) {
			dependThreads[req.tid].insert(r.tid);
		}
		if (req.type != LockType::R) {
			dependThreads[req.tid].insert(r.tid);
		}

		wait_for.push(r);
	}

	queue<int> checkDepend;
	for (int tid : dependThreads[req.tid]) {
		checkDepend.push(tid);
	}

	while (!checkDepend.empty()) {
		int cur = checkDepend.front();
		checkDepend.pop();

		if (cur == req.tid)
			return Status::deadlock;
		if (visited.find(cur) != visited.end())
			continue;

		for (int tid : dependThreads[cur])
			checkDepend.push(tid);
		visited.insert(cur);
	}

	lock_request[req.index].push_back(req);
	threads_info[req.tid][req.type] = req.index;

	//lock_req, only me => get lock
	if (lock_request[req.index].front() == req)
		return Status::get;

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
	
	
	/*int i = 0;
	cout << "-- dependency" << endl;
	for (set<int> d : dependThreads) {
	
		cout << "Thread " << i++ << " : ";
		for (int tid : d) {
			cout << tid << " ";
		}
		cout << endl;

	}*/

	return Status::waiting;
}

void Monitor::printRecordQueue(){
	int i = 0; 
	for (auto w : lock_request) {
		if (w.size() == 0) {
			i++;
			continue;
		}
		cout << "Record " << i++ << "(" << records[i].read() << ")"<< " : ";
		for (auto req : w) {
			cout << req.tid << "-" << req.type << " " << req.getLock << " | ";
		}
		cout << endl;
	}
	cout << "--------------------------------------------" << endl;
}

int Monitor::read(int index) {
	return records[index].read();
}

void Monitor::write(int index, int value) {
	records[index].set(value);
}
/*
void Monitor::operation(vector<LockInfo> req){
	int value;
	for(LockInfo r : req){
		if(r.type == LockType::R){
			value = records[r.index].read();
		} else if(r.type == LockType::W1){
			records[r.index].add(value + 1);
		} else if(r.type == LockType::W2){
			records[r.index].sub(value);
		}
	}
}*/

int Monitor::readRecord(int index) {
	return records[index].read();
}