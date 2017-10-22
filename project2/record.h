#ifndef __RECORD__ 
#define __RECORD__

#include <iostream>
#include <mutex>
#include <vector>
#include "rwlock.h"
#include "lockinfo.h"

using namespace std;

//class for save record
class Record {

public:
	//initial value is 100
	Record() : value(100) {};
	~Record() {};

	//set Index 0 to numRecords -1
	void setIndex(int _index){
		index = _index;
	}

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

	//return record value
	int read(){
		return value;
	}

	//set record value as arguments
	void set(int _value) {
		value = _value;
	}

private:
	int index;
	int64_t value;
	RWLock rwlock;
};

#endif