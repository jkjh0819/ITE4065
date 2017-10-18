#ifndef __RECORD__ 
#define __RECORD__

class Record {

public:
	Record() : value(100) {};
	~Record();

private:
	int64_t value;
};

#endif