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
	/*Record(int _index) : index(_index), value(100) {
		cout << index << endl;
	};*/
	~Record() {};

	void setIndex(int _index){
		index = _index;
	}

	void noticeCancel() {
		rwlock.noticeCancel();
	}

	void getReaderLock(vector<LockInfo>& waitings, LockInfo req){
		rwlock.lockShared(waitings, req);
		/*
		logMtx.lock();
		cout << req.tid << " : get reader lock of " << req.index << endl;
		logMtx.unlock();
		*/
	}

	void getWriterLock(vector<LockInfo>& waitings, LockInfo req){
		rwlock.lockExclusive(waitings, req);
		/*
		logMtx.lock();
		cout << req.tid << " : get writer lock of " << req.index << endl;
		logMtx.unlock();
		*/
	}

	void releaseReaderLock(){
		rwlock.unlockShared();
		/*
		logMtx.lock();
		cout << "release reader lock of " << getIndex() << endl;		
		logMtx.unlock();
		*/
	}

	void releaseWriterLock(){
		rwlock.unlockExclusive();
		/*
		logMtx.lock();
		cout << "release writer lock of " << getIndex() << endl;
		logMtx.unlock();
		*/
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

	int getIndex(){
		return index;
	}

private:
	int index;
	int64_t value;
	RWLock rwlock;

	
	//mutex logMtx;
};

#endif