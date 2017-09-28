#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

#define newline '\n'
#define MAX_ALPHA 26

class AhoCorasick {
	
public:
	AhoCorasick();
	AhoCorasick(int patternNum, int patternLen);
	~AhoCorasick();
	//void addWord(vector<string> word);
	//void makeGraph();
	void makeGraph(set<string> s_word);
	vector<string> search(string input);
	//void deleteWord(string word);

private:
	vector<vector<int> > graph;
	//vector<string> words;
	set<string> words;
	vector<string> result;
	set<int> del;
	int init_state;
	int patternNum;
	int state_num;
	int cur_size;
	int patternLen;
};