#ifndef __TRANSACTION__
#define __TRANSACTION__

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include "monitor.h"
#include "lockinfo.h"
#include "random.h"

using namespace std;

//class for handle Transaction
class Transaction {

public:	
	Transaction(int N, int R, int E);
	~Transaction();

	//run work threads
	void run();

	//transaction content
	void work(int tid);

	//release locks and roll back
	void undo(vector<LockInfo>& havings);

	//plus 1 and return order
	int getCommitId();

private:
	vector<thread> threads;

	int64_t global_execution_order;
	int64_t max_global_execution_order;
	int64_t numThreads;
	int64_t numRecords;

	//global
	mutex global_mutex;

	//check deadlock and get record lock
	Monitor * monitor; 

	//random number generator
	Random<int> r;
};

#endif 