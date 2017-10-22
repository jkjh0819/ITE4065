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

	void noticeCancel() {
		writer.notify_all();
	}

	void lockExclusive(vector<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		do {
			writer.wait(lock, [&] { return checkQueue(waitings, req); });
		}while(writing);
		writing = true;
		while( numReader > 0)
			reader.wait(lock);
	}

	void unlockExclusive(){
		{
			lock_guard<mutex> lock(mtx);
			writing = false;
		}
		writer.notify_all();
	}

	void lockShared(vector<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		do {
			writer.wait(lock, [&] { return checkQueue(waitings, req); });
		} while (writing);
		++numReader;
	}

	void unlockShared(){
		int local_numReader;
		bool local_writing;

		{
			lock_guard<mutex> _Lock(mtx);
			local_numReader = --numReader;
			local_writing = writing;
		}

		if(local_writing && local_numReader == 0)
			reader.notify_one();
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
		
		if(req == first)
			return true;

		if (req.type == LockType::R) {
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
			bool onlyWriter = false;
			for (LockInfo r : waitings) {
				if (r == req) {
					break;
				}
				//happen before req, if write lock exist and reader lock which not get lock
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