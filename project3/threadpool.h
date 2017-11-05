#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <thread>
#include <vector>
#include <chrono>
#include <climits>
#include <iostream>

#include "wfsnapshot.h"
#include "random.h"

//execute time
#define running 60000

using namespace std;

//class for managing threads and doing update
class ThreadPool{
public:
	/*initialize snapshot as _numThreads, exit as false, 
		updateCounts, Random value generator */
	ThreadPool(int _numThreads);
	~ThreadPool();

	//make threads and start work(update for 1 minute)
	int run();

	//update for 1 minute, thread id is parameter i
	void work(int i);

private:
	vector<thread> threads;
	int numThreads;

	//flag for exit
	bool exit;

	//snapshot variable
	WFSnapshot<int> * snapshot;

	//random value generator
	Random<int> r;

	//for return total update
	int * updateCounts;
};

#endif