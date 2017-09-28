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
	this->patternNum = patternNum;
	this->state_num = patternNum + 1;
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
			this->patternNum++;
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
	int init_state = patternNum, cur_state, final_state = 0;
	this->state_num = this->init_state + 1;
	if(this->cur_size < (this->patternNum + this->patternLen + 1)){
		this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (26, -1));
		this->cur_size = patternNum + patternLen * 2 + 1;
	}

	for(vector<string>::iterator it = words.begin(); it != words.end(); it++){
		int index;
		string cur_word = *it;

		cur_state = init_state;
		for(index = 0; index < cur_word.length();){
			if(graph[cur_state][cur_word[index]-'a'] == -1){
				break;
			} else {
				cur_state = graph[cur_state][cur_word[index]-'a'];
				index++;
			}
		}

		while(index < cur_word.length()){
			if(++index == cur_word.length()){
				graph[cur_state][cur_word[index]-'a'] = final_state++;
			} else {
				graph[cur_state][cur_word[index]-'a'] = state_num;
				cur_state = state_num++;
			}
 		}

	}
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
	int init_state = patternNum, cur_state;
	string s = "";

	cur_state = init_state;
	result.clear();

	for(int index = 0; index < input.length();){
		cur_state = graph[cur_state][input[index] - 'a'];
		s += input[index];
		if(cur_state < init_state) {
			result.push_back(s);
			index++;
		} else if (cur_state == -1) {
			s = "";
			index++;
			cur_state = init_state;
		} else {
			index++;
		}

	}

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
	int cur_state = init_state;

	vector<int>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != myVector.end()) {
    	for(int index = 0; index < word.length() - 1; index++){
			cur_state = graph[cur_state][word[index]-'a'];
		}

    	this->words.erase(position);
    	this->patternNum--;
	}
	

	
	
}

vector<string> AhoCorasick::retResult(){
	return this->result;
}
