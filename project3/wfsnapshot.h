#ifndef __WFSNAPSHOT__
#define __WFSNAPSHOT__

#include "stampedsnap.h"

#include <iostream>

using namespace std;

template <typename T>
class WFSnapshot {

public:
	WFSnapshot(int n) 
      : snaps(n) {
   	}

	void update(T value, int tid){
		vector<T> snap = scan();
		StampedSnap<T> oldValue = snaps[tid];
		StampedSnap<T> newValue = StampedSnap<T>(value, oldValue.get_stamp() + 1, snap);
		snaps[tid] = newValue;
	}

	vector<T> scan() {
      vector<StampedSnap<T> > oldCopy;
      vector<StampedSnap<T> > newCopy;
      vector<bool> moved(snaps.size(), false);

      oldCopy = collect();

      while (true) {
         newCopy = collect();

         bool flag = false;
         for (int i = 0; i < snaps.size(); i++) {
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

      vector<T> result(snaps.size());
      for (int i = 0; i < result.size(); i++) {
         result[i] = newCopy[i].read();
      }
      return result;
   }

private:
	vector<StampedSnap<T> > snaps;

	vector<StampedSnap<T> > collect() {
		//cout << snaps.size() << endl;
		vector<StampedSnap<T> > copy(snaps.size());

		for (int i = 0; i < copy.size(); i++) {
			copy[i] = snaps[i];
		}

		return copy;
	}
};

#endif