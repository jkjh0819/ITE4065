#ifndef __RECORD__ 
#define __RECORD__

#include <iostream>
#include <mutex>
#include <queue>
#include "rwlock.h"
#include "lockinfo.h"

using namespace std;

class Record {

public:
	Record(int _index) : index(_index), value(100) {};
	~Record() {};

	void readerLock(queue<LockInfo>& waitings, LockInfo req){
		rwlock.lockShared(waitings, req);
	}

	void writerLock(queue<LockInfo>& waitings, LockInfo req){
		rwlock.lockExclusive(waitings, req);
	}

	void set(int _value){
		value = _value;
	}

	void release(queue<LockInfo>& waitings, LockInfo req){

	}

private:
	int index;
	int64_t value;
	RWLock rwlock;
};

#endif