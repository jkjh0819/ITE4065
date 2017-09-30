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
	this->patternLen = patternLen;
}

AhoCorasick::~AhoCorasick(){

}
	
/*void AhoCorasick::addWord(vector<string> word){
	bool somethingAdded = false;

	for(vector<string>::iterator it = word.begin(); it != word.end(); it++){
			this->words.push_back(*it);
			this->patternLen += it->length();
			//this->patternNum++;
	}

	this->makeGraph();
}*/

/*
void AhoCorasick::makeGraph(){
	int cur_state, final_state = 0;

	this->init_state = words.size();
	this->patternNum = words.size();
	this->state_num = this->init_state + 1;
	if(this->cur_size < (this->patternNum + this->patternLen + 1)){
		this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));
		this->cur_size = patternNum + patternLen * 2 + 1;
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
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
				graph[cur_state][cur_word[index]-'a'] = state_num;
				if(index+1 == cur_word.length()){
					graph[cur_state][cur_word[index]-'a'] = final_state;
					swap(graph[state_num], graph[final_state]);
					final_state++;
					cur_state = state_num++;
				} else {
					cur_state = state_num++;
				}
			} else {
				if(index+1 == cur_word.length()){
					int next = graph[cur_state][cur_word[index]-'a'];
					swap(graph[next], graph[final_state]);
					graph[cur_state][cur_word[index]-'a'] = final_state++;
				}
				cur_state = graph[cur_state][cur_word[index]-'a'];
			}
			index++;
		}
	}	
}*/

void AhoCorasick::makeGraph(set<string> word_list, int patternLen){
	int cur_state, final_state = 0;

	this->init_state = word_list.size();
	this->patternNum = word_list.size();
	this->words = word_list;
	this->state_num = this->init_state + 1;
	
	if(this->cur_size < (this->patternNum + this->patternLen + 1)){
		this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));
		this->cur_size = patternNum + patternLen * 2 + 1;
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
	} else {
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
	}

	for(set<string>::iterator it = words.begin(); it != words.end(); it++){
		int index;
		string cur_word = *it;
		
		cur_state = init_state;

		for(index = 0; index < cur_word.length();){
			
			if(graph[cur_state][cur_word[index]-'a'] == -1){
				graph[cur_state][cur_word[index]-'a'] = state_num;
				if(index+1 == cur_word.length()){
					graph[cur_state][cur_word[index]-'a'] = final_state;
					swap(graph[state_num], graph[final_state]);
					final_state++;
					cur_state = state_num++;
				} else {
					cur_state = state_num++;
				}
			} else {
				if(index+1 == cur_word.length()){
					int next = graph[cur_state][cur_word[index]-'a'];
					swap(graph[next], graph[final_state]);
					graph[cur_state][cur_word[index]-'a'] = final_state++;
				}
				cur_state = graph[cur_state][cur_word[index]-'a'];
			}
			index++;
		}
	}
}

vector<string> AhoCorasick::search(string input){
	map<string,bool> check;

	for (auto w : words)
    	check.emplace(w, false);

	int cur_state = this->init_state;
	
	string s = "";

	result.clear();

	for(int start = 0; start < input.length();start++){
		s = "";
		cur_state = init_state;
		for(int index = start; index < input.length();){
			cur_state = graph[cur_state][input[index] - 'a'];
			s += input[index];

			if (cur_state == -1) {
				//s = "";
				//cur_state = init_state;
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
	return this->result;
}

/*	
void AhoCorasick::deleteWord(string word){
	int cur_state = this->init_state;

	vector<string>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != words.end()) {
		size_t index = distance(words.begin(), position);
		del.insert(index);
    	//this->words.erase(position);
    	//this->patternNum--;
	}
}*/

