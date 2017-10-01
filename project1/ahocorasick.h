#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <mutex>


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/move/move.hpp>

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
	void searchThread(int start);
	void deleteWord(string word);

private:
	mutex mtx;

	vector<vector<int> > graph;
	vector<string> words;
	vector<string> result;
	set<int> del;
	vector<vector<pair<int, string> > > finds;
	map<int, vector<string> > found;
	int init_state;
	int patternNum;
	int state_num;
	int cur_size;
	int patternLen;

	string query;


};