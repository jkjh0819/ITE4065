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
	/*
	statenum = init + 1
	if cur_size < pattern num + patternlen + 1 => resize
	loop from start word to end word
		cur_state = init
		length = 0
		loop for find end of graph log
			if graph[cur_state][cur_word[index]] == -1 => break
			else cur_state = graph[cur_state][cur_word[index]], length++
		loop to end of cur word
			graph[cur_state++][cur_word[index]] = length++
			if cur_word at end => graph[that word index][cur word end] = length
	*/
}

int AhoCorasick::search(string input){
	/*
	cur = init
	cur string = ""
	loop from start of input to end of input
		cur = graph[cur][input[index]]
		if( cur < init) => accept, register depth at result[string]
		else if(cur == -1) start at next input[index], string = ""  ex> string , at r -1, start from i


	*/

}
	
void AhoCorasick::deleteWord(string word){
	vector<int>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != myVector.end())
    	this->words.erase(position);
	
}
