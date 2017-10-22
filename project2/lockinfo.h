#ifndef __LOCKINFO__
#define __LOCKINFO__

//R is reader lock for record i
//W1 is writer lock for record j
//W2 is writer lock for record k
enum LockType { R, W1, W2 };


struct LockInfo{
	//record index
	int index;

	//type of lock 
	LockType type;

	//lock request thread id
	int tid;

	//whether get lock or not
	bool getLock;

	bool operator==(const LockInfo& a) const {
		return (index == a.index && type == a.type && tid == a.tid);
	}

};

#endif