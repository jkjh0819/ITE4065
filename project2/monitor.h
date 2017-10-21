#ifndef __MONITOR__
#define __MONITOR__

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include "lockinfo.h"
#include "record.h"

#define UNDEFINED -1

using namespace std;

class Monitor{
public:
	Monitor(int numThreads, int numRecords);
	~Monitor();

	void getLock(LockInfo req);
	void cancelLock(LockInfo req);
	void deleteLock(LockInfo req);
	void releaseLock(LockInfo req);

	//return true -> deadlock, return false -> not deadlock
	bool deadlock_check(LockInfo req);
	int readRecord(int index);
	void operation(vector<LockInfo> req);

	void printRecordQueue();

private:
	//thread status table
	vector<vector<int> > threads_info;
	
	//thread request queue
	vector<vector<LockInfo> > lock_request;

	Record * records;


	mutex logMtx;
};

#endif