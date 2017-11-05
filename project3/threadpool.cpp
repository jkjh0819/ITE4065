#include "threadpool.h"

ThreadPool::ThreadPool(int _numThreads){
	numThreads = _numThreads;
	exit = false;
	//initialize snapshot with numThreads
	snapshot = new WFSnapshot<int>(numThreads);

	//random generator generate value between MIN_INT and MAX_INT
	r = Random<int>(numeric_limits<int>::min(), numeric_limits<int>::max());

	//make updateCount array for calculating total update after exit
	updateCounts = new int[numThreads];
}

ThreadPool::~ThreadPool(){
	delete[] updateCounts;
	delete snapshot;
}

int ThreadPool::run(){
	int sumUpdateCount = 0;

	//make threads which execute work function
	for(int i = 0; i < numThreads; i++){
		threads.push_back(thread(&ThreadPool::work, this, i));
	}

	//waiting running milliseconds, current running time is 1 minute
	this_thread::sleep_for(chrono::milliseconds(running));

	//set flag
	exit = true;

	//wait all threads finished
	for(int i = 0; i < numThreads; i++){
		threads[i].join();
	}

	//calculate total update count
	for(int i = 0; i < numThreads; i++)
		sumUpdateCount += updateCounts[i];

	return sumUpdateCount;
}


void ThreadPool::work(int i){
	//set thread id and initialize update count
	int tid = i;
	int updateCount = 0;

	//during 1 minute(before flag is set)
	while(!exit){
		//get random number
		int newValue = r.get();

		//update random number
		this->snapshot->update(newValue, tid);

		//increase updateCount
		updateCount++;
	}

	//save update count
	updateCounts[tid] = updateCount;
}
