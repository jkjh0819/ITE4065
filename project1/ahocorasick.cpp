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
	map<string, bool> check;
	for (auto w : words)
		check.emplace(w, false);
	this->query = input;
	found.clear();
	finds.clear();
	result.clear();

	boost::asio::io_service io;
	boost::thread_group threads;
	boost::asio::io_service::work work(io);
	for (int i = 0; i < boost::thread::hardware_concurrency(); ++i)
	{
		threads.create_thread(boost::bind(&boost::asio::io_service::run, &io));
	}
	for (int start = 0; start < input.length(); start++) {
		io.post(boost::bind(&AhoCorasick::searchThread, this, start));
	}
	io.stop();
	threads.join_all();

	for (auto f : finds) {
		for (auto d : f) {
			this->found[d.first].push_back(d.second);
		}
	}

	compare c;
	for (auto f : found) {
		if (f.second.size() != 0) {
			sort(f.second.begin(), f.second.end(), c);
			for (auto w : f.second) {
				if (!check[w]) {
					result.push_back(w);
					check[w] = true;
				}
			}
		}
	}
	return this->result;
}


void AhoCorasick::searchThread(int start) {
	map<string, bool> check;
	int cur_state = this->init_state;
	string s = "";
	vector<pair<int, string> > find;

	for (auto w : words)
		check.emplace(w, false);
	for (int pos = start; pos < this->query.length(); pos++) {
		s = "";
		cur_state = this->init_state;
		for (int index = pos; index < this->query.length();) {
			cur_state = this->graph[cur_state][this->query[index] - 'a'];
			s += this->query[index];
			if (cur_state == -1) {
				break;
			}
			else if (cur_state < this->init_state) {
				if (check[s] != true && this->del.find(cur_state) == this->del.end()) {
					find.push_back(make_pair(pos, s));
					check[s] = true;
				}
				index++;
			}
			else {
				index++;
			}
		}
	}
	mtx.lock();
	finds.push_back(find);
	mtx.unlock();
}


void AhoCorasick::deleteWord(string word){
	int cur_state = this->init_state;

	for(vector<string>::iterator it = words.begin(); it != words.end(); it++){
		if(*it == word) {
			size_t index = distance(words.begin(), it);
			del.insert(index);
		}
	}
}

