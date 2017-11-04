#ifndef __STAMPEDSNAP__
#define __STAMPEDSNAP__

#include <vector>

using namespace std;

template<typename T>
class StampedSnap{

public:
	StampedSnap(T value = 0, long stamp = 0,
         const std::vector<T>& snap = std::vector<T>())
         : value(value), snap(snap), stamp(stamp) {
      }

    StampedSnap(const StampedSnap& ssnap)
         : value(ssnap.value), stamp(0) {
         snap = ssnap.snap;
    }
	
	T read() const {
		return value;
	}

	vector<T> get_snap() const {
		return snap;
	}

	long get_stamp() const {
		return stamp;
	}

private:
	long stamp;
	T value;
	vector<T> snap;
};

#endif