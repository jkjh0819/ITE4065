#ifndef __STAMPEDSNAP__
#define __STAMPEDSNAP__

#include <vector>

using namespace std;

//class for saving stamp and managing value
template<typename T>
class StampedSnap{

public:
	//basic constructor
	StampedSnap(T value = 0, long stamp = 0,
         T* _snap = nullptr)
         : value(value), snap(_snap), stamp(stamp) {
      }

     //update snapshot
    StampedSnap(const StampedSnap& ssnap)
         : value(ssnap.value), stamp(0) {
         snap = ssnap.snap;
    }
	
	//return value
	T read() const {
		return value;
	}

	//return snapshot
	T * get_snap() const {
		return snap;
	}

	//return stamp
	long get_stamp() const {
		return stamp;
	}

private:
	//when update this snapshot
	long stamp;

	T value;

	//snapshot container
	T * snap;
};

#endif