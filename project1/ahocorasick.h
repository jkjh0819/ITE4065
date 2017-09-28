#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define newline '\n'
#define MAX_ALPHA 26

class AhoCorasick {
	
public:
	AhoCorasick();
	AhoCorasick(int patternNum, int patternLen);
	~AhoCorasick();
	void addWord(vector<string> word);
	void makeGraph();
	int search(string input);
	void deleteWord(string word);
	vector<string> retResult();

private:
	vector<vector<int> > graph;
	vector<string> words;
	vector<string> result;
	int patternNum;
	int state_num;
	int cur_size;
	int patternLen;
}