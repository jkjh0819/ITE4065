#include <iostream>
#include <sstream>
#include <mutex>
#include "record.h"
#include "transaction.h"

using namespace std;

int main(int argc, char * argv[]){
	int N, R, E;

	//check parameter
	if(argc < 4){
		cout << "Please run as ./run N R E" << endl;
	} else {
		istringstream ssN(argv[1]);
		istringstream ssR(argv[2]);
		istringstream ssE(argv[3]);

		if(!(ssN >> N)){
			cerr << "Invalid input " << argv[1] << '\n';
		} else if(!(ssR >> R)) {
			cerr << "Invalid input " << argv[2] << '\n';
		} else if(R < 3){
			cerr << "R should be more than 3" << '\n';
		} else if(!(ssE >> E)){
			cerr << "Invalid input " << argv[3] << '\n';
		}

		//make transaction
		Transaction t(N, R, E);

		//start transaction
		t.run();
	}

	return 0;
}