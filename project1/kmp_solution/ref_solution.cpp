#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <thread>
#include <algorithm>
#include <pthread.h>  


pthread_mutex_t  mu = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

vector<int> getPi(string p);
void* kmp(string word);

/*struct Param {
    string word;
    int start;
};*/

map<string, int> result;
string input;

int main(){
    int N;
    set<string> word_list;
    char cmd;
    string buf;

    std::ios::sync_with_stdio(false);

    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> buf;
        word_list.insert(buf);
    }
    cout << "R" << std::endl;

    while(cin >> cmd){
        cin.get();
        getline(cin, buf);
        switch(cmd){
            case 'Q':
                {
                    thread * threads = new thread[word_list.size()];
                    int i = 0, count;
                    //struct Param *p;
                    input = buf;

                    for(set<string>::iterator it = word_list.begin();it != word_list.end();){
                        for (count = 0; count < 47 && it != word_list.end(); it++, count++){

                            /*if((p = (struct Param*)malloc(sizeof(*p))) == NULL){
                                fprintf(stderr,"MALLOC THREAD_PARAM ERROR");
                                return (-1);
                            }
                            //p->word = *it;
                            //p->start = -1;
                            */
                            string word = *it;
                            threads[count] = thread(kmp, word);
                        }

                        for(i = 0; i < count; i++){
                            threads[i].join();
                        }
                    }

                    auto cmp = [](std::pair<string, int> const & a, std::pair<string, int> const & b) 
                    { 
                         return a.second != b.second?  a.second < b.second : a.first < b.first;
                    };

                    bool find = false;
                    vector<pair<string,int> > v(result.begin(), result.end());
                    sort(v.begin(), v.end(), cmp);
                    vector<pair<string, int> >::iterator it = v.begin();

                    for(int cnt = result.size(); cnt != 0; cnt--, it++){
                        //cout << it-> first << " " << it->second << endl;
                        if(it->second != -1) {
                            find = true;
                            cout << it->first;
                            if(cnt != 1) {
                                cout << "|";
                            }
                        }
                    }
                    if (!find) {
                        cout << -1;
                    }
                    cout << std::endl;
                    result.clear();
                }
                break;
            case 'A':
                word_list.insert(buf);
                break;
            case 'D':
                word_list.erase(buf);
                break;
        }
    }
    return 0;
}


vector<int> getPi(string p){ 
    int m = p.length(), j=0; 
    vector<int> pi(m, 0); 
    for(int i = 1; i< m ; i++){ 
        while(j > 0 && p[i] != p[j]) 
            j = pi[j-1]; 
        if(p[i] == p[j]) 
            pi[i] = ++j; 
    } 
        return pi; 
} 

void* kmp(string word){
    //struct Param *readParams = (struct Param*)param;
    //string s = input;
    //string p = word;
    //cout << word << "|";

    auto pi = getPi(word);    
    int n = input.length(), m = word.length(), j =0; 
    for(int i = 0 ; i < n ; i++){ 
        while(j>0 && input[i] != word[j]) 
            j = pi[j-1]; 
        if(input[i] == word[j]){ 
            if(j==m-1){
                pthread_mutex_lock(&mu);
                result[word] = i-m+1;
                pthread_mutex_unlock(&mu); 

                return NULL;
            } else { 
                j++; 
            } 
        } 
    }
    pthread_mutex_lock(&mu);
    result[word] = -1;   
    pthread_mutex_unlock(&mu);  

    return NULL;
}