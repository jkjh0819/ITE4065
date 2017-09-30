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
	for(vector<string>::iterator it = word.begin(); it != word.end(); it++){
			this->words.push_back(*it);
			this->patternLen += it->length();
	}

	this->makeGraph();
}

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

	
}

vector<string> AhoCorasick::search(string input){
	map<string,bool> check;

	for (auto w : words)
    	check.emplace(w, false);

	//int cur_state = this->init_state;
	//string s = "";
	vector<string> result;
	this->query = input;

	found.clear();
	//result.clear();

	int i,j;

	//for(int start = 0; start < input.length();start++){
	for(int start = 0; start < query.length();){
		//이 부분이 스레드 화 되어야 함.
		//각 길이별로 작업 큐에 들어가고, 그 작업큐가 끝나면 종료
		//check가 <int, vector<string> >이 되고 ...
		//각 스레드에서 final state에 도달하면 check[start_pos].push_back(s)
		//스레드 실행이 모두 끝나면 모든 check key에 대해 if(check[key].size()!= 0 ) => sort by length,
		// result.push_back(value)
		vector<thread> searchThreads;

		for(i = 0; i < thread::hardware_concurrency()*2 + 1 && start < input.length(); i++){
			searchThreads.push_back(thread(&AhoCorasick::searchThread, this ,start++));
		}

		for (j = 0; j < searchThreads.size() ; j++){
			if(searchThreads[j].joinable()){
        		searchThreads[j].join();
			}
 		}

		/*s = "";
		cur_state = init_state;

		for(int index = start; index < query.length();){
			cur_state = graph[cur_state][input[index] - 'a'];
			s += input[index];

			if (cur_state == -1) {
				break;
			} else if(cur_state < init_state) {
				if(check[s] != true && del.find(cur_state) == del.end()){
					result.push_back(s);
					check[s] = true;
				}
				index++;
			} else {
				index++;
			}
		}*/
		searchThreads.clear();
	}

	compare c;

	for(auto f : found){
		if(f.second.size() != 0){
			sort(f.second.begin(), f.second.end(), c);
			for(auto w : f.second){
				if(!check[w]) {
					result.push_back(w);
					check[w] = true;
				}
			}
		}
	}

	//return this->result;
	return result;
}

void AhoCorasick::searchThread(int start){
	
	
	map<string,bool> check;
	string s;
	int cur_state = this->init_state;

	/*for(vector<string>::iterator it = this->words.begin(); it != this->words.end(); it++){
		check.emplace(*it, false);
	}*/
	for (auto w : this->words)
    	check.emplace(w, false);

	for(int pos = start; pos < this->query.length(); pos++){
		s = "";
		cur_state = this->init_state;

		for(int index = pos; index < this->query.length();){
			cur_state = this->graph[cur_state][this->query[index] - 'a'];
			s += this->query[index];

			if (cur_state == -1) {
				break;
			} else if(cur_state < this->init_state) {
				if(check[s] != true && this->del.find(cur_state) == this->del.end()){
					this->mtx.lock();
					this->found[pos].push_back(s);
					this->mtx.unlock();
					check[s] = true;
				}
				index++;
			} else {
				index++;
			}
		}
	}
}


void AhoCorasick::deleteWord(string word){
	int cur_state = this->init_state;

	for(vector<string>::iterator it = words.begin(); it != words.end(); it++){
		if(*it == word) {
			size_t index = distance(words.begin(), it);
			del.insert(index);
		}
	}

	/*vector<string>::iterator position = find(this->words.begin(), this->words.end(), word);
	if (position != words.end()) {
		size_t index = distance(words.begin(), position);
		del.insert(index);
	}*/
}

