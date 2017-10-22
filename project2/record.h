#ifndef __RECORD__ 
#define __RECORD__

#include <iostream>
#include <mutex>
#include <vector>
#include "rwlock.h"
#include "lockinfo.h"


static mutex logMtx;

using namespace std;

class Record {

public:
	Record() : value(100) {};
	~Record() {};

	void setIndex(int _index){
		index = _index;
	}

	void noticeCancel() {
		rwlock.noticeCancel();
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

	int read(){
		return value;
	}

	void set(int _value) {
		value = _value;
	}

	int getIndex(){
		return index;
	}

private:
	int index;
	int64_t value;
	RWLock rwlock;
};

#endif