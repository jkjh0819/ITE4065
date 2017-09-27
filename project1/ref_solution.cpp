#include <iostream>
#include <string>
#include <set>
#include <map>
#include "ahocorasick.h"

using namespace std;

int main(){
    int N;
    set<string> word_list;  //current words
    char cmd;
    string buf;
    AhoCorasick FSA;
    vector<string> words;  //temporary buffer
    int patterNum = 0, patternLen = 0;

    std::ios::sync_with_stdio(false);

    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> buf;
        //words.push_back(buf);
        patterNum++;
        patternLen += buf.length();
        word_list.insert(buf);
    }

    words = vector(word_list.begin(), word_list.end());
    FSA = FSA(patterNum, patternLen);
    FSA.addWord(words);

    cout << "R" << std::endl;

    while(cin >> cmd){
        cin.get();
        getline(cin, buf);
        switch(cmd){
            case 'Q':
                {
                    if(!words.empty()){
                        FSA.addWord(words);
                        words.clear();
                    }
                    FSA.search();
                   /* multimap<size_t, string> result;
                    for (set<string>::iterator it = word_list.begin();
                            it != word_list.end(); it++){
                        size_t pos = buf.find(*it);
                        if (pos != string::npos){
                            result.insert(make_pair(pos, *it));
                        }
                    }
                    multimap<size_t, string>::iterator it = result.begin();
                    for (int cnt = result.size(); cnt != 0; cnt--, it++){
                        cout << it->second;
                        if (cnt != 1){
                            cout << "|";
                        }
                    }
                    cout << std::endl;*/
                }
                break;
            case 'A':
                if(word_list.find(buf) != word_list.end()){
                    words.push_back(buf);
                }
                //word_list.insert(buf);
                break;
            case 'D':
                FSA.deleteWord(buf);
                word_list.erase(buf);
                break;
        }
    }
    return 0;
}
