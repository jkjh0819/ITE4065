#ifndef __RWLOCK__
#define __RWLOCK__

#include <mutex>
#include <condition_variable>
#include <queue>
#include "lockinfo.h"
#include <iostream>

using namespace std;

class RWLock {

public:
	RWLock() : mtx(), reader(), writer(), numReader(0), writing(false) {};
	~RWLock(){};

	void lockExclusive(queue<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		while(writing)
			writer.wait(lock, [&]{ return checkQueue(waitings, req);});
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

	void lockShared(queue<LockInfo>& waitings, LockInfo req){
		unique_lock<mutex> lock(mtx);
		while(writing)
			writer.wait(lock, [&]{ return checkQueue(waitings, req);});
		
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
		else if(!local_writing && local_numReader == -2)
			writer.notify_all();

	}

private:
	mutex mtx;
	condition_variable reader, writer;
	int numReader;
	bool writing;

	bool checkQueue(queue<LockInfo>& waitings, LockInfo req){
		LockInfo first = waitings.front();
		cout << "first : " << first.type << endl;
		if(req.tid == first.tid && req.type == first.type)
			return true;
		return false;
	}
};

#endif