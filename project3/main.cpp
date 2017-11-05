#include <iostream>
#include <sstream>

#include "threadpool.h"

int main(int argc, char * argv[]){
	int N;

	//check parameter
	if(argc < 2){
		cout << "Please run as ./run N" << endl;
	} else {
		istringstream ssN(argv[1]);

		if(!(ssN >> N)){
			cerr << "Invalid input " << argv[1] << '\n';
		} 

		//make Thread for doing update
		ThreadPool t(N);

		//print update count after run update for 1 minute.
		cout << "update : " << t.run() << endl;

	}	

	return 0;
}