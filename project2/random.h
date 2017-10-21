#ifndef __RANDOM__
#define __RANDOM__

#include <random>

using namespace std;

class Random{
public:
	Random(int min, int max) : gen(random_device()), dis(min, max) {};

	int get(){
		return dis(gen);
	}

private:
	mt19337 gen;
	uniform_int_distribution<> dis;
}

#endif