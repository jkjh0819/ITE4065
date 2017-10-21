#ifndef __RANDOM__
#define __RANDOM__

#include <random>
#include <vector>
#include <unordered_set>

#define numGet 3

using namespace std;

template<typename T>
class Random{
public:
	Random() {};
	Random(T min, T max) : gen(random_device()()), dis(min, max) {};

	vector<T> get(){
		unordered_set<T> ret;
		while(ret.size() < numGet) {
			T t = dis(gen);
			if(ret.find(t) == ret.end())
				ret.insert(t);
		}
		vector<T> v(ret.begin(), ret.end());
		return v;
	}

private:
	mt19937 gen;
	uniform_int_distribution<> dis;
};

#endif