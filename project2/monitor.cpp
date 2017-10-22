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
	//cout << records[req.index]->getIndex() << endl;

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
		/*logMtx.lock();
		cout << "try release reader lock of " << req.index << endl;
		logMtx.unlock();*/

		records[req.index].releaseReaderLock();
	} else {
		/*logMtx.lock();
		cout << "try release writer lock of " << req.index << endl; 
		logMtx.unlock();*/
		records[req.index].releaseWriterLock();
	}

	

	cout << "after release" << endl;
	printRecordQueue();
}

bool Monitor::deadlock_check(LockInfo req) {
	//cout << records[req.index]->getIndex() << endl;
	cout << ">> request " << req.tid << "-" << req.type << " to " << req.index << endl;

	/*LockInfo cur;
	stack<LockInfo> wait_for;
	for (LockInfo r : lock_request[req.index])
		wait_for.push(r);

	while (!wait_for.empty()) {
		cur = wait_for.top();
		wait_for.pop();

		//find cycle
		if (cur.tid == req.tid) {
			return true;
		}

		if (cur.type == LockType::W1) {
			for (LockInfo r : lock_request[threads_info[cur.tid][LockType::R]]) {
					if (r.tid == cur.tid)
						break;
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}
					wait_for.push(r);
				}


		} else if (cur.type == LockType::W2) {

				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
					if (r.tid == cur.tid)
						break;
					if (r.tid == req.tid)
						return true;
					wait_for.push(r);
				}
		}
	}*/
	/*if (req.type == LockType::R) {
		lock_request[req.index].push_back(req);
		threads_info[req.tid][req.type] = req.index;

		printRecordQueue();
		return false;
	}*/

	LockInfo cur;
	stack<LockInfo> wait_for;
	set<int> visited;

	for (LockInfo r : lock_request[req.index]) {
		//preThreads.insert(r.tid);
		if (req.type == LockType::R && r.type != LockType::R) {
			//visited.insert(r.tid);
			dependThreads[req.tid].insert(r.tid);
		}
		if (req.type != LockType::R) {
			dependThreads[req.tid].insert(r.tid);
			//visited.insert(r.tid);
		}

		wait_for.push(r);
	}

	//for (int tid : dependThreads[req.tid])
	//	visited.insert(tid);

	while (!wait_for.empty()) {
		cur = wait_for.top();
		wait_for.pop();

		/*for (int tid : dependThreads[cur.tid]) {
			if (visited.find(tid) != visited.end())
				return true;
			visited.insert(tid);
			//dependThreads[req.tid].insert(tid);
		}*/

		for (int tid : dependThreads[cur.tid])
			dependThreads[req.tid].insert(tid);

		//current dependThreads include reqThreads
		if (dependThreads[cur.tid].find(req.tid) != dependThreads[cur.tid].end())
			return true;
	

		//find cycle
		if (cur.tid == req.tid) {
			return true;
		}

		if (cur.type == LockType::R) {
			if (cur.getLock) {
				//have lock, check W1
				if (threads_info[cur.tid][LockType::W1] != UNDEFINED)
				{

					for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
						if (r.tid == cur.tid && r.getLock) {
							wait_for.push(r);
							break;
						}
					}
				}
			}
			else {
				//doesn't have lock, should satisfy preRequest
				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::R]]) {
					if (r.tid == cur.tid) {
						//wait_for.push(r);
						break;
					}
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}
					//when lock is read, if there are any write graph, it depends on the thread commit
					if (r.type != LockType::R) {
						dependThreads[req.tid].insert(r.tid);
					}

					wait_for.push(r);
				}
			}


		}
		else if (cur.type == LockType::W1) {
			if (cur.getLock) {
				//have lock, check W1
				if (threads_info[cur.tid][LockType::W2] != UNDEFINED)
				{

					for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W2]]) {
						if (r.tid == cur.tid && r.getLock) {
							wait_for.push(r);
							break;
						}
					}
				}
			}
			else {
				//doesn't have lock, should satisfy preRequest
				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
			
					if (r.tid == cur.tid) {
						//wait_for.push(r);
						break;
					}
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}
					dependThreads[req.tid].insert(r.tid);

					wait_for.push(r);
				}
			}
		}
		else {
			if (cur.getLock) {
				break;
			}
			else {
				//doesn't have lock, should satisfy preRequest
				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W2]]) {
					if (r.tid == cur.tid) {
						//wait_for.push(r);
						break;
					}
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}
					dependThreads[req.tid].insert(r.tid);

					wait_for.push(r);
				}
			}

		}
	}
	/*if (cur.getLock) {
		if (cur.type == LockType::R) {
			if (threads_info[cur.tid][LockType::W1] != UNDEFINED)
			{
				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
					if (r.tid == cur.tid && r.getLock) {
						break;
					}
				}

				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
					if (r.tid == cur.tid) {
						wait_for.push(r);
						break;
					}
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}

					wait_for.push(r);

				}
			}

		}
		else if (cur.type == LockType::W1) {
			if (threads_info[cur.tid][LockType::W2] != UNDEFINED)
			{
				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W2]]) {
					if (r.tid == cur.tid && r.getLock) {
						break;
					}
				}

				for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W2]]) {
					if (r.tid == cur.tid) {
						wait_for.push(r);

						break;
					}
					if (r.tid == req.tid) {
						cout << ">> find cycle" << endl;
						return true;
					}

					wait_for.push(r);

				}
			}
		}
	}
	else {
		if (cur.type == LockType::W2) {

			for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
				if (r.tid == cur.tid) {
					if (!r.getLock)
						wait_for.push(r);
					break;
				}
				if (r.tid == req.tid) {
					cout << ">> find cycle" << endl;
					return true;
				}

				wait_for.push(r);

			}


		}
		else if (cur.type == LockType::W1) {

			for (LockInfo r : lock_request[threads_info[cur.tid][LockType::R]]) {
				if (r.tid == cur.tid) {
					if(!r.getLock)
						wait_for.push(r);

					break;
				}
				if (r.tid == req.tid) {
					cout << ">> find cycle" << endl;
					return true;
				}

				wait_for.push(r);

			}

		}
	}
}



	/*else if (cur.type == LockType::W2) {
		for (LockInfo r : lock_request[threads_info[cur.tid][LockType::R]]) {
			if (r.tid == req.tid)
				return true;
			if (r.tid == cur.tid)
				break;

		}
		for (LockInfo r : lock_request[threads_info[cur.tid][LockType::W1]]) {
			if (r.tid == req.tid)
				return true;
			if (r.tid == cur.tid)
				break;
		}
	}*/

	queue<int> checkDepend;
	for (int tid : dependThreads[req.tid]) {
		checkDepend.push(tid);
	}

	while (!checkDepend.empty()) {
		int cur = checkDepend.front();
		checkDepend.pop();

		if (cur == req.tid)
			return true;
		if (visited.find(cur) != visited.end())
			continue;

		for (int tid : dependThreads[cur])
			checkDepend.push(tid);
		visited.insert(cur);
	}

	lock_request[req.index].push_back(req);
	threads_info[req.tid][req.type] = req.index;

	printRecordQueue();
	int i = 0;
	cout << "-- dependency" << endl;
	for (set<int> d : dependThreads) {
	
		cout << "Thread " << i++ << " : ";
		for (int tid : d) {
			cout << tid << " ";
		}
		cout << endl;

	}

	return false;

}

void Monitor::printRecordQueue(){
	int i = 0; 
	for (auto w : lock_request) {
		if (w.size() == 0) {
			i++;
			continue;
		}
		cout << "Record " << i++ << " : ";
		for (auto req : w) {
			cout << req.tid << "-" << req.type << " " << req.getLock << " | ";
		}
		cout << endl;
	}
	cout << "--------------------------------------------" << endl;
}

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
}

int Monitor::readRecord(int index){
	return records[index].read();
}
