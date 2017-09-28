#include "ahocorasick.h"

AhoCorasick::AhoCorasick(){
	this->graph.resize(1, vector<int> (MAX_ALPHA, -1));
	this->init_state = 0;
	this->patternNum = 0;
	this->state_num = 1;
	this->cur_size = 1;	
	this->patternLen = 0;
}
	
AhoCorasick::AhoCorasick(int patternNum, int patternLen){
	this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));
	this->init_state = 0;
	this->patternNum = 0;
	this->state_num = 1;
	this->cur_size = patternNum + patternLen * 2 + 1;
	this->patternLen = 0;
}

AhoCorasick::~AhoCorasick(){

}
	
void AhoCorasick::addWord(vector<string> word){
	bool somethingAdded = false;

	for(vector<string>::iterator it = word.begin(); it != word.end(); it++){
			this->words.push_back(*it);
			this->patternLen += it->length();
			this->patternNum++;
	}

	del.clear();
	this->makeGraph();
}

void AhoCorasick::makeGraph(){
	int cur_state, final_state = 0;
	this->init_state = patternNum;
	this->state_num = this->init_state + 1;
	if(this->cur_size < (this->patternNum + this->patternLen + 1)){
		this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));
		this->cur_size = patternNum + patternLen * 2 + 1;
	} else {
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
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
			if(index+1 == cur_word.length()){
				graph[cur_state][cur_word[index]-'a'] = final_state++;
			} else {
				graph[cur_state][cur_word[index]-'a'] = state_num;
				cur_state = state_num++;
			}
			index++;
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

vector<string> AhoCorasick::search(string input){
	map<string,bool> check;

	for (auto w : words)
    	check.emplace(w, false);

	int cur_state = this->init_state;
	
	string s = "";

	result.clear();

	for(int start = 0; start < input.length();start++){
		for(int index = start; index < input.length();){
			cur_state = graph[cur_state][input[index] - 'a'];
			s += input[index];

			if (cur_state == -1) {
				s = "";
				cur_state = init_state;
				break;
			} else if(cur_state < init_state) {
				if(del.find(cur_state) == del.end() && check[s] != true){
					result.push_back(s);
					check[s] = true;
				}
				index++;
			} else {
				index++;
			}
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
	return this->result;
}
	
void AhoCorasick::deleteWord(string word){
	int cur_state = this->init_state;

	vector<string>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != words.end()) {
		size_t index = distance(words.begin(), position);
		del.insert(index);
    	this->words.erase(position);
    	this->patternNum--;
	}
}

