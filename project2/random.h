#ifndef __RANDOM__
#define __RANDOM__

#include <random>
#include <vector>
#include <unordered_set>

//the number of random number to select
#define numGet 3

using namespace std;

//class for generate random number 
template<typename T>
class Random{
public:
	Random() {};
	Random(T min, T max) : gen(random_device()()), dis(min, max) {};

	vector<T> get(){
		unordered_set<T> ret;
		while(ret.size() < numGet) {
			//generate a random number
			T t = dis(gen);

			//if it is distinct, insert to return set
			if(ret.find(t) == ret.end())
				ret.insert(t);
		}
		//for easy access, return as vector
		vector<T> v(ret.begin(), ret.end());
		return v;
	}

private:
	mt19937 gen;
	uniform_int_distribution<> dis;
};

#endif