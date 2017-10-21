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

	ofstream outFile("thread" + to_string(tid) + ".txt");

	do {

	vector<int> v = r.get();
	int i=v[0], j=v[1], k=v[2], cid;
	int i_value, j_value, k_value;
	LockInfo req;
	vector<LockInfo> havingLocks;

	//random get different i,j,k

	//get global lock
	global_mutex.lock();
	req = {i, LockType::R, tid};
	if(monitor->deadlock_check(req)){
		undo(havingLocks);
		global_mutex.unlock();
		continue;
	}
	//lose global lock
	global_mutex.unlock();
	havingLocks.push_back(req);

	//get global lock
	global_mutex.lock();
	req = {j, LockType::W1, tid};
	if(monitor->deadlock_check(req)){
		undo(havingLocks);
		global_mutex.unlock();
		continue;
	}
	//lose global lock
	global_mutex.unlock();
	havingLocks.push_back(req);

	///get global lock
	global_mutex.lock();
	req = {k, LockType::W2, tid};
	if(monitor->deadlock_check(req)){
		undo(havingLocks);
		global_mutex.unlock();
		continue;
	}
	//lose global lock
	global_mutex.unlock();
	havingLocks.push_back(req);

	for(LockInfo r : havingLocks)
		monitor->getLock(r);

	//get global lock
	global_mutex.lock();
	//all rw lock lose

	//order++ and get it
	cid = getCommitId();

	//log add
	if(cid <= max_global_execution_order){
		monitor->operation(havingLocks);

		outFile << cid << " " << i << " " << j << " " << k << " "
					<< monitor->readRecord(i) << " " << monitor->readRecord(j) << " " << monitor->readRecord(k) << endl;

		cout << tid << " : commit " << cid << " finished" << endl; 
	}
	
	for(LockInfo r : havingLocks)
		monitor->releaseLock(r);

	//lose global lock
	global_mutex.unlock();

	} while(global_execution_order < max_global_execution_order);

	outFile.close();
}

void Transaction::undo(vector<LockInfo>& havings){
	for(LockInfo r : havings)
		monitor->deleteLock(r);
}

int Transaction::getCommitId(){
	global_execution_order++;
	return global_execution_order;
}

