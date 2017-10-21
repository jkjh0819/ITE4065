#ifndef __RECORD__ 
#define __RECORD__

#include <iostream>
#include <mutex>
#include <vector>
#include "rwlock.h"
#include "lockinfo.h"

using namespace std;

class Record {

public:
	Record(int _index) : index(_index), value(100) {};
	~Record() {};

	void getReaderLock(vector<LockInfo>& waitings, LockInfo req){
		rwlock.lockShared(waitings, req);
	}

	void getWriterLock(vector<LockInfo>& waitings, LockInfo req){
		rwlock.lockExclusive(waitings, req);
	}

	void releaseReaderLock(){
		rwlock.unlockShared();
	}

	void releaseWriterLock(){
		rwlock.unlockExclusive();
	}

	int read(){
		return value;
	}

	void add(int _value){
		value += _value;
	}

	void sub(int _value){
		value -= _value;
	}

	void release(vector<LockInfo>& waitings, LockInfo req){

	}

private:
	int index;
	int64_t value;
	RWLock rwlock;
};

#endif