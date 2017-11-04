#ifndef __WFSNAPSHOT__
#define __WFSNAPSHOT__

#include "stampedsnap.h"

#include <iostream>

using namespace std;

template <typename T>
class WFSnapshot {

public:
	WFSnapshot(int n) {
		snapSize = n;
		snaps = new StampedSnap<T>[snapSize];
   	}

	void update(T value, int tid){
		T * snap = scan();
		StampedSnap<T> oldValue = snaps[tid];
		StampedSnap<T> newValue = StampedSnap<T>(value, oldValue.get_stamp() + 1, snap);
		snaps[tid] = newValue;
	}

	T * scan() {
      StampedSnap<T> * oldCopy;
      StampedSnap<T> * newCopy;
      vector<bool> moved(snapSize, false);

      oldCopy = collect();

      while (true) {
         newCopy = collect();

         bool flag = false;
         for (int i = 0; i < snapSize; i++) {
            if (oldCopy[i].get_stamp() != newCopy[i].get_stamp()) {
               if (moved[i]) {
                  return oldCopy[i].get_snap();
               }
               else {
                  moved[i] = true;
                  oldCopy = newCopy;
                  flag = true;
                  break;
               }
            }
         }
         if (!flag) break;
      }

      T * result = new T[snapSize];
      for (int i = 0; i < snapSize; i++) {
         result[i] = newCopy[i].read();
      }
      return result;
   }

private:
	StampedSnap<T> * snaps;
	int snapSize;

	StampedSnap<T> * collect() {
		//cout << snaps.size() << endl;
		StampedSnap<T> * copy = new StampedSnap<T>[snapSize];

		for (int i = 0; i < snapSize; i++) {
			copy[i] = snaps[i];
		}

		return copy;
	}
};

#endif