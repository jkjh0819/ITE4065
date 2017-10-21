#ifndef __MONITOR__
#define __MONITOR__

#include <vector>
#include <queue>
#include "lockinfo.h"
#include "record.h"

#define UNDEFINED -1

using namespace std;

class Monitor{
public:
	Monitor(int numThreads, int numRecords);
	~Monitor();

	int get(LockInfo req);
	void set(int _value);
	void release(LockInfo req);

	//return true -> deadlock, return false -> not deadlock
	bool deadlock_check(int record, LockInfo request);
	LockInfo getFirstReq(int record);

private:
	//thread status table
	vector<vector<int> > threads_info;
	
	//thread request queue
	vector<queue<LockInfo> > lock_request;

	Record ** records;
};

#endif