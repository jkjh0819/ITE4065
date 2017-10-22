#include "transaction.h"

Transaction::Transaction(int N, int R, int E){
	global_execution_order = 0;
	max_global_execution_order = E;
	numThreads = N;
	numRecords = R;
	monitor = new Monitor(numThreads, numRecords);
	r = Random<int>(0, numRecords-1);

}

Transaction::~Transaction(){

}

void Transaction::run(){
	for(int i = 0; i < numThreads; i++){
		threads.push_back(thread(&Transaction::work, this, i));
	}

	for(int i = 0; i < numThreads; i++){
		threads[i].join();
	}
}

void Transaction::work(int tid){

	ofstream outFile("thread" + to_string(tid+1) + ".txt");

	while (global_execution_order < max_global_execution_order) {

		vector<int> v = r.get();
		int i = v[0], j = v[1], k = v[2], cid;
		int i_value, j_value, k_value;
		LockInfo req;
		vector<LockInfo> havingLocks;

		//record i
		global_mutex.lock();
		req = { i, LockType::R, tid, false };
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
			continue;
		case Status::waiting:
			global_mutex.unlock();
			monitor->getLock(req);
			global_mutex.lock();
			break;
		case Status::get:
			monitor->getLock(req);
			break;
		}
		global_mutex.unlock();

		havingLocks.push_back(req);

		i_value = monitor->read(req);


		//record j

		global_mutex.lock();
		req = { j, LockType::W1, tid, false };
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
			undo(havingLocks);
			global_mutex.unlock();
			continue;
		case Status::waiting:
			global_mutex.unlock();
			monitor->getLock(req);
			global_mutex.lock();
			break;
		case Status::get:
			monitor->getLock(req);
			break;
		}
		global_mutex.unlock();
		havingLocks.push_back(req);
		j_value = monitor->read(req);
		monitor->write(req, j_value + i_value + 1);

		//record k

		global_mutex.lock();
		req = { k, LockType::W2, tid, false };
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
			monitor->write(req, j_value);
			undo(havingLocks);
			global_mutex.unlock();
			continue;
		case Status::waiting:
			global_mutex.unlock();
			monitor->getLock(req);
			global_mutex.lock();
			break;
		case Status::get:
			monitor->getLock(req);
			break;
		}
		global_mutex.unlock();
		havingLocks.push_back(req);
		k_value = monitor->read(req);
		monitor->write(req, k_value - i_value);


		//get global lock
		global_mutex.lock();
		//all rw lock lose

		for (LockInfo r : havingLocks)
			monitor->releaseLock(r);


		//order++ and get it
		cid = getCommitId();

		//log add
		if (cid <= max_global_execution_order) {

			outFile << cid << " " << i << " " << j << " " << k << " "
				<< monitor->readRecord(i) << " " << monitor->readRecord(j) << " " << monitor->readRecord(k) << endl;

			cout << tid << " : commit " << cid << " finished" << endl;
		}
		else {
			//rollback
			for (LockInfo r : havingLocks) {
				switch (r.type) {
				case LockType::R :
						break;
				case LockType::W1:
					monitor->write(r, j_value);
					break;
				case LockType::W2:
					monitor->write(r, k_value);
				}
			}
			global_mutex.unlock();
			return;
		}
		global_mutex.unlock();
	}

	outFile.close();
}

void Transaction::undo(vector<LockInfo>& havings){
	for (LockInfo r : havings) {
		monitor->releaseLock(r);
	}
}

int Transaction::getCommitId(){
	global_execution_order++;
	return global_execution_order;
}

