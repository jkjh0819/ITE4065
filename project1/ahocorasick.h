#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

//std::endl make speed slow, using '\n' is faster
#define newline '\n'

//according to project spec, input is composed of small letter
#define MAX_ALPHA 26

class AhoCorasick {
	
public:
	/**
	* get enough memory to make AhoCorasick graph 
	*/
	AhoCorasick();
	AhoCorasick(int patternNum, int patternLen);
	~AhoCorasick();

	/**
	* add word to this->words, calculate patternLen 
	* @param: word, added word list vector
	*/
	void addWord(vector<string> word);
	
	/**
	* based on this->words, make graph in 2d-array
	* each node are separated by row index
	* each edge are separated by col index, used ascii a-0 to z-25
	*/
	void makeGraph();
	
	/**
	* search matched pattern
	* if current state is under init_state, that pattern is matched.
	* @param: input, query string for searching 
	* @return: result, found patterns
	*/
	vector<string> search(string input);

	/**
	* add index of deleted word to this->del
	* @param: word, string for deleting
	*/
	void deleteWord(string word);

private:
	vector<vector<int> > graph;
	vector<string> words;
	vector<string> result;

	//include index of deleted word
	set<int> del;

	//used for search word
	int init_state;
	int patternNum;
	int state_num;

	//used for handling memory
	int cur_size;
	int patternLen;
};