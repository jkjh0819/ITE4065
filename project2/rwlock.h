#ifndef __RWLOCK__
#define __RWLOCK__

#include <mutex>
#include <condition_variable>
#include <vector>
#include "lockinfo.h"
#include <iostream>

using namespace std;

class RWLock {

public:
	RWLock() : mtx(), reader(), writer(), numReader(0), writing(false) {};
	~RWLock(){};

	//get writer lock
	void lockExclusive(vector<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		//if req is first writer lock in queue, set the flag
		do {
			writer.wait(lock, [&] { return checkQueue(waitings, req); });
		}while(writing);
		writing = true;
		//if reader is remain, wait
		while( numReader > 0)
			reader.wait(lock);
	}

	//release writer lock
	void unlockExclusive(){
		{
			lock_guard<mutex> lock(mtx);
			//set flag down
			writing = false;
		}
		//notify all to request in queue
		writer.notify_all();
	}

	//get reader lock
	void lockShared(vector<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		//if req is before first writer lock request, get readlock
		do {
			writer.wait(lock, [&] { return checkQueue(waitings, req); });
		} while (writing);
		++numReader;
	}

	//release reader lock
	void unlockShared(){
		int local_numReader;
		bool local_writing;

		{
			lock_guard<mutex> _Lock(mtx);
			local_numReader = --numReader;
			local_writing = writing;
		}

		//if writer lock wait for reader being zero, notify to the writer lock 
		if(local_writing && local_numReader == 0)
			reader.notify_one();
		//there is no writer request, notify to writer
		else if(!local_writing)
			writer.notify_all();
	}

private:
	mutex mtx;
	condition_variable reader, writer;
	int numReader;
	bool writing;

	bool checkQueue(vector<LockInfo>& waitings, LockInfo req){
		LockInfo first = waitings.front();
		
		//if request is the first of queue
		if(req == first)
			return true;

		//if request is reader lock request
		if (req.type == LockType::R) {
			//happen before request is all reader, return true
			//else return false
			bool allReader = false;
			for (LockInfo r : waitings) {
				if (r == req) {
					break;
				}
				if (r.type != LockType::R)
					return allReader;
			}
			allReader = true;
			return allReader;
		}
		else {
			//if request is writer lock request
			bool onlyWriter = false;
			for (LockInfo r : waitings) {
				if (r == req) {
					break;
				}
				//Among happen before request, there is write lock or reader lock which not get lock, return false
				//else return true
				if (r.type == LockType::W1 || r.type == LockType::W2 || !r.getLock)
					return onlyWriter;
			}
			onlyWriter = true;
			return onlyWriter;
		}

		return false;
	}
};

#endif