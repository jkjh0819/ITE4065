#include "monitor.h"

Monitor::Monitor(int numThreads, int numRecords){

	records = (Record **)malloc(sizeof(Record*));
	for(int i = 0; i < numRecords; i++){
		records[i] = new Record(i);
	}

	threads_info.resize(numThreads, vector<int>(3, UNDEFINED));
	lock_request.resize(numRecords, vector<LockInfo>());
}


Monitor::~Monitor(){

}

int Monitor::getLock(LockInfo req){
	if(req.type == LockType::R){
		records[req.index]->getReaderLock(lock_request[req.index],req);
	} else {
		records[req.index]->getWriterLock(lock_request[req.index],req);
	}
}

void Monitor::deleteLock(LockInfo req){
	vector<LockInfo>::iterator it;

  	it = find (lock_request[req.index].begin(), lock_request[req.index].end(), req);
  	if(it != lock_request[req.index].end())
  		lock_request[req.index].erase(it);

	threads_info[req.tid][req.type] = UNDEFINED;
}

void Monitor::releaseLock(LockInfo req){
	if(req.type == LockType::R){
		records[req.index]->releaseReaderLock();
	} else {
		records[req.index]->releaseWriterLock();
	}

	deleteLock(req);
}

bool Monitor::deadlock_check(LockInfo req){
	LockInfo cur;
	stack<LockInfo> wait_for;
	for(LockInfo r : lock_request[req.index])
		wait_for.push(r);

	while(!wait_for.empty()){
		cur = wait_for.top();
		
		//find cycle
		if(cur.tid == req.tid){
			return true;
		}

		if(cur.type == LockType::R){
			if(threads_info[cur.tid][LockType::W1] != UNDEFINED)
			{
				for(LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]){
					if(r == cur)
						break;
					wait_for.push(r);
				}
			}

		} else if(cur.type == LockType::W1){
			if(threads_info[cur.tid][LockType::W2] != UNDEFINED)
			{
				for(LockInfo r : lock_request[threads_info[cur.tid][LockType::W2]]){
					if(r == cur)
						break;
					wait_for.push(r);
				}
			}
		} 
		wait_for.pop();
	}

	lock_request[req.index].push_back(req);
	threads_info[req.tid][req.type] = req.index;

	return false;
}

void Monitor::operation(vector<LockInfo> req){
	int value;
	for(LockInfo r : req){
		if(r.type == LockType::R){
			value = records[r.index]->read();
		} else if(r.type == LockType::W1){
			records[r.index]->add(value + 1);
		} else if(r.type == LockType::W2){
			records[r.index]->sub(value);
		}
	}
}

int Monitor::readRecord(int index){
	return records[index]->read();
}
