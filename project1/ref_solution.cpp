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
        patterNum++;
        patternLen += buf.length();
        word_list.insert(buf);
    }

    words = vector<string>(word_list.begin(), word_list.end());
    FSA = AhoCorasick(patterNum, patternLen);
    FSA.addWord(words);
    words.clear();

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
                    vector<string> result = FSA.search(buf);
                   
                    vector<string>::iterator it = result.begin();
                    if(result.size() == 0){
                        cout << "-1" << newline;
                    } else {
                        for(int cnt = result.size(); cnt != 0; cnt--, it++){
                            cout << *it;
                            if(cnt != 1) {
                                cout << "|";
                            }
                        }
                        cout << newline;
                    }
                }
                break;
            case 'A':
                if(word_list.find(buf) == word_list.end()){
                    words.push_back(buf);
                    word_list.insert(buf);
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
