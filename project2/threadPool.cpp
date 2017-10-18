#include "threadPool.h"

ThreadPool::ThreadPool(int N, int E){
	global_execution_order = 0;
	max_global_execution_order = E;
	numThreads = N;

	for(int i = 0; i < numThreads; i++){
		threads.push_back(thread(&ThreadPool::work, this, i));
	}

}

ThreadPool::~ThreadPool(){

}

void ThreadPool::run(){
	
	for(int i = 0; i < threads.size(); i++){
		threads[i].join();
	}
}

void ThreadPool::work(int tid){
	int i,j,k;


	//random get different i,j,k

	//get global lock
	//get readerlock about i
	//lose global lock

	//get global lock
	//get writerlock about j
	//lose global lock

	//get global lock
	//get writerlock about k
	//lose global lock

	//do something about i, j, k

	//get global lock
	//all rw lock lose
	//order++ and get it
	//log add
	//lose global lock

}
