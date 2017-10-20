#ifndef __LOCKINFO__
#define __LOCKINFO__

enum LockType { R, W1, W2 };

typedef struct {
	int index;
	int type;
	int tid;
} LockInfo;

#endif