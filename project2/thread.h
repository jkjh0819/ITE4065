#ifndef __THREAD__ 
#define __THREAD__

#include <thread>

using namespace std;

class Thread {
	
public:
	Thread();
	~Thread()

	int getTid(){
		return tid;
	}

private:
	thread t;
	int tid;
};

#endif