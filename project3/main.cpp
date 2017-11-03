#include <iostream>
#include <sstream>

#include "threadpool.h"

int main(int argc, char * argv[]){
	int N;

	if(argc < 2){
		cout << "Please run as ./run N" << endl;
	} else {
		istringstream ssN(argv[1]);

		if(!(ssN >> N)){
			cerr << "Invalid input " << argv[1] << '\n';
		} 

		ThreadPool t(N);

		cout << "update : " << t.run();

	}	

	return 0;
}