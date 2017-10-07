#include <iostream>
#include <string>
#include <set>
#include <map>
#include "ahocorasick.h"

using namespace std;

int main(){
    int N;

    //init words, sync with latest word status
    set<string> word_list; 

    char cmd;
    string buf;
    AhoCorasick FSA;

    //buffer, use for update word
    vector<string> words;  

    //for initialize FSA
    int patterNum = 0, patternLen = 0;

    std::ios::sync_with_stdio(false);

    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> buf;
        patterNum++;
        patternLen += buf.length();
        word_list.insert(buf);
    }

    //make initial FSA
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
                    //if some word added, add to FSA
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

                //if new word not in current word, add it
                if(word_list.find(buf) == word_list.end()){
                    words.push_back(buf);
                    word_list.insert(buf);
                }
                break;
            case 'D':
            
                //delete word from FSA and current word list
                FSA.deleteWord(buf);
                word_list.erase(buf);
                break;
        }
    }
    return 0;
}
