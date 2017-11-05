#ifndef __RANDOM__
#define __RANDOM__

#include <random>

using namespace std;

//class for generate random number 
template<typename T>
class Random{
public:
	Random() {};
	Random(T min, T max) : gen(random_device()()), dis(min, max) {};

	//return random T value
	T get(){
		T ret = dis(gen);
		return ret;
	}

private:
	mt19937 gen;
	uniform_int_distribution<> dis;
};

#endif