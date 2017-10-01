#ifndef AHOCORASICK
#define AHOCORAISCK

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

#include "threadpool.h"

using namespace std;

#define newline '\n'
#define MAX_ALPHA 26

struct compare {
	bool operator()(const std::string& first, const std::string& second) {
		return first.size() < second.size();
	}
};

class AhoCorasick {
	
public:
	AhoCorasick();
	AhoCorasick(int patternNum, int patternLen);
	~AhoCorasick();
	void addWord(vector<string> word);
	void makeGraph();
	vector<string> search(string input);
	void deleteWord(string word);

private:
	vector<vector<int> > graph;
	vector<string> words;
	vector<string> result;
	set<int> del;
	int init_state;
	int patternNum;
	int state_num;
	int cur_size;
	int patternLen;

	map<string, int> found;
	map<int, vector<string> > found_r;

	string query;

	ThreadPool * pool;
};

#endif