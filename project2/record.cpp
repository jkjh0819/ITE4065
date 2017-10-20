#include "record.h"

Record::~Record(){

}

int Record::get(Monitor& monitor, LockInfo req){

	cout << "try lock " << req.type << endl;

	if(req.type == LockType::R){
		rwlock.lockShared(index, monitor, req);
	} else {
		rwlock.lockExclusive(index, monitor, req);
	}
	//shared_lock<shared_mutex> lock(mtx);
	
}

void Record::set(int _value){
	//unique_lock<shared _mutex> lock(mtx);
	value = _value;
}

void Record::release(Monitor& monitor, LockInfo req){
	
}