#include "transaction.h"

Transaction::Transaction(int N, int R, int E){
	global_execution_order = 0;
	max_global_execution_order = E;
	numThreads = N;
	numRecords = R;

	//initialize monitor
	monitor = new Monitor(numThreads, numRecords);

	//initialize random generator
	r = Random<int>(0, numRecords-1);
}

Transaction::~Transaction(){

}

//make threads and wait for return
void Transaction::run(){
	for(int i = 0; i < numThreads; i++){
		threads.push_back(thread(&Transaction::work, this, i));
	}

	for(int i = 0; i < numThreads; i++){
		threads[i].join();
	}
}


void Transaction::work(int tid){

	ofstream outFile("./thread" + to_string(tid+1) + ".txt");

	//until max global execution order(input E)
	while (global_execution_order < max_global_execution_order) {

		//get random number
		vector<int> v = r.get();

		//set record number
		int i = v[0], j = v[1], k = v[2], cid;

		//prev values
		int i_value, j_value, k_value;
		LockInfo req;
		vector<LockInfo> havingLocks;

		//record i

		//get global lock
		global_mutex.lock();
		req = { i, LockType::R, tid, false };

		//according to result of deadlock check
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
		//when reader lock, do not need rollback
		//because there is any lock to get
			continue;
		case Status::waiting:
		//if req is in waiting queue, unlock global lock
			global_mutex.unlock();
			//wait for get reader lock
			monitor->getLock(req);
			//wait for get global lock
			global_mutex.lock();
			break;
		case Status::get:
			//get reader lock
			monitor->getLock(req);
			break;
		}
		//unlock global lock
		global_mutex.unlock();

		//save lock info
		havingLocks.push_back(req);

		//read value of i
		i_value = monitor->read(i);


		//record j

		//get global lock
		global_mutex.lock();
		req = { j, LockType::W1, tid, false };
		//according to result of deadlock check
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
		//writer lock 1 change no value
		//just release readlock
			undo(havingLocks);
		//unlock global lock
			global_mutex.unlock();
			continue;
		case Status::waiting:
		//if req is in waiting queue, unlock global lock
			global_mutex.unlock();
			//wait for get writer lock 1
			monitor->getLock(req);
			//wait for get global lock
			global_mutex.lock();
			break;
		case Status::get:
			//get writer lock 1
			monitor->getLock(req);
			break;
		}
		//unlock global lock
		global_mutex.unlock();

		//save lock info
		havingLocks.push_back(req);

		//read value of j
		j_value = monitor->read(j);

		//set value as j += i + 1
		monitor->write(j, j_value + i_value + 1);

		//record k

		//get global lock
		global_mutex.lock();
		req = { k, LockType::W2, tid, false };
		switch (monitor->deadlock_check(req)) {
		case Status::deadlock:
			//roll back value to j 
			monitor->write(j, j_value);
			//release locks
			undo(havingLocks);
			//unlock global lock
			global_mutex.unlock();
			continue;
		case Status::waiting:
			//unlock global lock
			global_mutex.unlock();
			//wait for get writer lock 2
			monitor->getLock(req);
			//wait for get global lock
			global_mutex.lock();
			break;
		case Status::get:
			//get writer lock 2
			monitor->getLock(req);
			break;
		}
		//unlock global lock
		global_mutex.unlock();

		//save lock info
		havingLocks.push_back(req);

		//read value of k
		k_value = monitor->read(k);

		//set value as k -= i
		monitor->write(k, k_value - i_value);
	

		//get global lock
		global_mutex.lock();
		
		//all rw lock release
		for (LockInfo r : havingLocks)
			monitor->releaseLock(r);

		//get order
		cid = getCommitId();

		//commit
		if (cid <= max_global_execution_order) {

			outFile << cid << " " << i << " " << j << " " << k << " "
				<< monitor->read(i) << " " << monitor->read(j) << " " << monitor->read(k) << endl;

				
			cout << tid << " : commit " << cid << " finished" << endl;
		}
		else {
			//if order is over maximum order
			//rollback
			
			monitor->write(i, i_value);
			monitor->write(j, j_value);
			monitor->write(k, k_value);

			//unlock global lock
			global_mutex.unlock();
			outFile.close();
			//thread exit
			return;
		}
		//unlock global lock
		global_mutex.unlock();
	}

	outFile.close();
}

//release rw locks
void Transaction::undo(vector<LockInfo>& havings){
	for (LockInfo r : havings) {
		monitor->releaseLock(r);
	}
}

//plus commit Id and get commit Id 
int Transaction::getCommitId(){
	global_execution_order++;
	return global_execution_order;
}

