#ifndef __TRANSACTION__
#define __TRANSACTION__

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include "monitor.h"
#include "record.h"
#include "lockinfo.h"

using namespace std;

class Transaction {

public:	
	Transaction(int N, int R, int E);
	~Transaction();

	void run();
	void work(int tid);
	void undo(vector<LockInfo>& havings);
	int getCommitId();

private:
	vector<thread> threads;

	int64_t global_execution_order;
	int64_t max_global_execution_order;
	int64_t numThreads;
	int64_t numRecords;
	mutex global_mutex;
	Monitor * monitor; 
};

#endif 