#ifndef __MONITOR__
#define __MONITOR__

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <set>
#include "lockinfo.h"
#include "record.h"

#define UNDEFINED -1

using namespace std;

//lock status 
//get - get lock immediately
//waiting - wait for other lock complete
//deadlock - make cycle -> deadlock create
enum Status {
	get, waiting, deadlock
};

//class for handle lock info and record
class Monitor{
public:
	Monitor(int numThreads, int numRecords);
	~Monitor();

	//get lock
	void getLock(LockInfo req);

	//delete lock info from queue
	void deleteLock(LockInfo req);

	//release lock
	void releaseLock(LockInfo req);

	//return whether new request make deadlock or not
	Status deadlock_check(LockInfo req);

	int read(int index);
	void write(int index, int value);

private:
	//thread status table
	vector<vector<int> > threads_info;
	
	//record lock request queue
	vector<vector<LockInfo> > lock_request;

	Record * records;

	//dependency graph
	vector<set<int> > dependThreads;
};

#endif