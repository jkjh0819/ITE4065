#include "threadpool.h"

ThreadPool::ThreadPool(int _numThreads){
	numThreads = _numThreads;
	exit = false;
	snapshot = new WFSnapshot<int>(numThreads);

	r = Random<int>(numeric_limits<int>::min(), numeric_limits<int>::max());
	updateCounts = new int[numThreads];
}

ThreadPool::~ThreadPool(){
	delete[] updateCounts;
	delete snapshot;
}

int ThreadPool::run(){
	int sumUpdateCount = 0;

	for(int i = 0; i < numThreads; i++){
		threads.push_back(thread(&ThreadPool::work, this, i));
	}

	cout << "create finished" << endl;
	this_thread::sleep_for(chrono::milliseconds(running));
	exit = true;

	for(int i = 0; i < numThreads; i++){
		threads[i].join();
	}

	for(int i = 0; i < numThreads; i++)
		sumUpdateCount += updateCounts[i];

	return sumUpdateCount;
}


void ThreadPool::work(int i){
	int tid = i;
	int updateCount = 0;

	while(!exit){
		int newValue = r.get();

		this->snapshot->update(newValue, tid);
		updateCount++;
	}

	updateCounts[tid] = updateCount;
}
