#ifndef __LOCKINFO__
#define __LOCKINFO__

enum LockType { R, W1, W2 };

struct LockInfo{
	int index;
	int type;
	int tid;
	bool getLock;

	bool operator==(const LockInfo& a) const {
		return (index == a.index && type == a.type && tid == a.tid);
	}
};

#endif