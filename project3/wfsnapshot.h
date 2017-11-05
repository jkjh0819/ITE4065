#ifndef __WFSNAPSHOT__
#define __WFSNAPSHOT__

#include "stampedsnap.h"

using namespace std;

//class for make snapshot
template <typename T>
class WFSnapshot {

public:
	//get snapSize(number of threads) and make snapshot array for each Thread
	WFSnapshot(int n) {
		snapSize = n;
		snaps = new StampedSnap<T>[snapSize];
   	}

   	//update value owned by each thread
	void update(T value, int tid){
		//find change history before update
		T * snap = scan();

		//get previous snapshot
		StampedSnap<T> oldValue = snaps[tid];

		//make new snapshot, increase stamp representing update time
		StampedSnap<T> newValue = StampedSnap<T>(value, oldValue.get_stamp() + 1, snap);
		
		//save new snapshot
		snaps[tid] = newValue;
	}

	/* 	find update happened
		it returns first collected snapshot(no update)
			or second collected snapshot(threads did update once)
			or snapshot of the thread did update twice */
	T * scan() {
      	StampedSnap<T> * oldCopy;
      	StampedSnap<T> * newCopy;
      	vector<bool> moved(snapSize, false);

      	//get all snapshot
      	oldCopy = collect();

      	while (true) {

      		//get all snapshot
	    	newCopy = collect();

			bool flag = false;

			//check whether update is happend
			for (int i = 0; i < snapSize; i++) {
				if (oldCopy[i].get_stamp() != newCopy[i].get_stamp()) {
					/* if moved is true, it means that thread updated twice
						return that thread's snapshot */
				    if (moved[i]) {
				      	return oldCopy[i].get_snap();
					}
					/* current checking thread is changed, set moved flag 
						and execute from collect again */
					else {
						moved[i] = true;
						oldCopy = newCopy;
						flag = true;
						break;
					}
				}
			}

			//if flag is false, it means that there isn't any update
    		if (!flag) break;
    	}

    	//make new snapshot
		T * result = new T[snapSize];
		for (int i = 0; i < snapSize; i++) {
			result[i] = newCopy[i].read();
		}
		return result;
	}

private:
	StampedSnap<T> * snaps;
	int snapSize;

	//return all current snapshots
	StampedSnap<T> * collect() {
		StampedSnap<T> * copy = new StampedSnap<T>[snapSize];

		for (int i = 0; i < snapSize; i++) {
			copy[i] = snaps[i];
		}

		return copy;
	}
};

#endif