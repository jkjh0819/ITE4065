#include "ahocorasick.h"

AhoCorasick::AhoCorasick(){
	//if any value not supported, just get memory as size of MAX_ALPHA
	this->graph.resize(1, vector<int> (MAX_ALPHA, -1));

	//other value will be set at makeGraph time
	this->init_state = 0;
	this->patternNum = 0;
	this->state_num = 1;
	this->cur_size = 1;	
	this->patternLen = 0;
}
	
AhoCorasick::AhoCorasick(int patternNum, int patternLen){
	//max graph size is pattternNum(final states) and patternLen(other states) + 1(initial states)
	//assign extra memory as patternLen
	this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));

	//other value will be set at makeGraph time
	this->init_state = 0;
	this->patternNum = 0;
	this->state_num = 1;
	this->cur_size = patternNum + patternLen * 2 + 1;
	this->patternLen = 0;
}

AhoCorasick::~AhoCorasick(){

}
	
void AhoCorasick::addWord(vector<string> word){

	for(vector<string>::iterator it = word.begin(); it != word.end(); it++){
			this->words.push_back(*it);
			this->patternLen += it->length();
	}

	this->makeGraph();
}

void AhoCorasick::makeGraph(){
	//cur_state follows pre-made route
	//final_state represent end of used final states
	int cur_state, final_state = 0;

	//all words have each final states, so initial states index is words.size()
	this->init_state = words.size();
	this->patternNum = words.size();

	//state_num represents end of used nodes
	this->state_num = this->init_state + 1;

	//if current memory size is smaller than max graph size, resize
	//and graph status initialize to -1(not connected)
	if(this->cur_size < (this->patternNum + this->patternLen + 1)){
		this->graph.resize(patternNum + patternLen * 2 + 1, vector<int> (MAX_ALPHA, -1));
		this->cur_size = patternNum + patternLen * 2 + 1;
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
	} else {
		for (auto &row : graph)
    		fill(row.begin(), row.end(), -1);
	}

	//make graph about all words
	for(vector<string>::iterator it = words.begin(); it != words.end(); it++){
		int index;
		string cur_word = *it;
		
		cur_state = init_state;

		for(index = 0; index < cur_word.length();){
			
			//no route, set edge to new node and renew state_num
			if(graph[cur_state][cur_word[index]-'a'] == -1){
				graph[cur_state][cur_word[index]-'a'] = state_num;

				//if index reach end of string, make edge to new final state
				if(index+1 == cur_word.length()){
					graph[cur_state][cur_word[index]-'a'] = final_state;
					swap(graph[state_num], graph[final_state]);
					final_state++;
					cur_state = state_num++;
				} else {
					cur_state = state_num++;
				}
			} else {
				//if route already set, follow and update cur_state 

				//follow until end of string, change that state row with new final state row
				//set edge to new final state
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
	//whether found word or not
	map<string,bool> check;

	//initialize check map
	for (auto w : words)
    	check.emplace(w, false);

	int cur_state = this->init_state;
	
	string s = "";

	result.clear();

	for(int start = 0; start < input.length();start++){
		//s represent current following string
		s = "";
		cur_state = init_state;

		for(int index = start; index < input.length();){
			//get next state by following graph edge
			cur_state = graph[cur_state][input[index] - 'a'];
			//add to current string
			s += input[index];

			//cur_state -1 means there is no edge, break for starting next position
			if (cur_state == -1) {
				break;
			} else if(cur_state < init_state) {
				//under init_state means that it is final states

				//if not found and not deleted, add to result
				if(check[s] != true && del.find(cur_state) == del.end()){
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
	
void AhoCorasick::deleteWord(string word){

	for(vector<string>::iterator it = words.begin(); it != words.end(); it++){
		//after find word position to delete, save index(distance) to this->del
		if(*it == word) {
			size_t index = distance(words.begin(), it);
			del.insert(index);
		}
	}
}

