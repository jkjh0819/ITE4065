#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "monitor.h"
#include "record.h"

using namespace std;

class ThreadPool {

public:	
	ThreadPool(int N, int E);
	~ThreadPool();

	void run();
	void work(int tid);

private:
	vector<thread> threads;

	int64_t global_execution_order;
	int64_t max_global_execution_order;
	int64_t numThreads;
	mutex global_mutex;
	Monitor monitor; 
};

#endif 