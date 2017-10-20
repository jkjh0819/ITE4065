#include "transaction.h"

Transaction::Transaction(int N, int R, int E){
	global_execution_order = 0;
	max_global_execution_order = E;
	numThreads = N;
	numRecords = R;
	monitor = new Monitor(numThreads, numRecords);
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
	int i=0, j=1, k=2, cid;
	int i_value, j_value, k_value;
	LockInfo req;
	vector<LockInfo> havingLocks;

	//random get different i,j,k

	//get global lock
	global_mutex.lock();
	req = {LockType::R, tid};
	if(monitor.deadlock_check(i, req)){
		global_mutex.unlock();
		continue;
	}
	//lose global lock
	global_mutex.unlock();

	//get readerlock about i
	monitor.get(i, req);
	havingLocks.push_back(req);

	//get global lock
	global_mutex.lock();
	req = {LockType::W1, tid};
	if(monitor.deadlock_check(j, req)){
		global_mutex.unlock();
		continue;
	}
	//lose global lock
	global_mutex.unlock();

	monitor.get(j, req);
	havingLocks.push_back(req);

	///get global lock
	global_mutex.lock();
	req = {LockType::W2, tid};
	//monitor.deadlock_check(j, req);
	//lose global lock
	global_mutex.unlock();

	monitor.get(k, req);
	havingLocks.push_back(req);

	//do something about i, j, k

	//get global lock
	global_mutex.lock();
	//all rw lock lose

	//order++ and get it
	cid = getCommitId();

	//log add
	if(cid <= max_global_execution_order){
		outFile << cid << " " << i << " " << j << " " << k << " "
					<< i_value << " " << j_value << " " << k_value << endl;
	}
	
	//lose global lock
	global_mutex.unlock();

	} while(global_execution_order < max_global_execution_order);
}

void Transaction::undo(vector<LockInfo>& havings){
	for(int i = 0; i < havings.size(); i++){

	}
}

int Transaction::getCommitId(){
	global_execution_order++;
	return global_execution_order;
}
