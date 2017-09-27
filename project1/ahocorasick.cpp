#include "ahocorasick.h"

AhoCorasick::AhoCorasick(){
	this->graph.resize(3, vector<int> (26, -1));
	this->init_state = 1;
	this->state_num = 2;
	this->cur_size = 3;	
	this->patternLen = 0;
}
	
AhoCorasick::AhoCorasick(int patternNum, int patternLen){
	this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (26, -1));
	this->init_state = patternNum;
	this->state_num = init_state + 1;
	this->cur_size = patternNum + patternLen * 2 + 1;
	this->patternLen = patternLen;
}

AhoCorasick::~AhoCorasick(){

}
	
void AhoCorasick::addWord(vector<string> word){
	bool somethingAdded = false;

	for(vector<string>::iterator it = this->words.begin(); it != this->words.end(); it++){
		if (find(this->words.begin(), this->words.end(), word) != this->words.end()) {
			this->words.push_back(word);
			this->patternLen += *it.length();
			somethingAdded = true;
		}
		else {
			continue;
		}
	}
	if(somethingAdded) {
		this->makeGraph();
	}
}

void AhoCorasick::makeGraph(){

}

int AhoCorasick::search(string input){

}
	
void AhoCorasick::deleteWord(string word){
	vector<int>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != myVector.end())
    	this->words.erase(position);
	
}
