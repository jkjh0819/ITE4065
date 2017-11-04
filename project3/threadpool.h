#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <thread>
#include <vector>
#include <chrono>
#include <climits>
#include <iostream>

#include "wfsnapshot.h"
#include "random.h"

#define running 20000

using namespace std;

class ThreadPool{
public:
	ThreadPool(int _numThreads);
	~ThreadPool();

	int run();
	void work(int i);

private:
	vector<thread> threads;

	int numThreads;
	bool exit;

	WFSnapshot<int> * snapshot;
	Random<int> r;

	int * updateCounts;
};

#endif