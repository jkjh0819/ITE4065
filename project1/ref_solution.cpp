#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <pthread.h> 

using namespace std;

vector<int> getPi(string p);
void* kmp(void * param);

struct Param {
    string input;
    string word;
};

template<template <typename> class P = std::less >

struct compare_pair_second {

    template<class T1, class T2> bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right) {

        return P<T2>()(left.second, right.second);

    }

};

map<string, int> result;

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
                    cout << "marker1" << endl;
                    pthread_t * threads = new pthread_t[word_list.size()];
                    vector<struct Param*> pointers;
                    int i = 0;
                    struct Param *p;

                    for (set<string>::iterator it = word_list.begin(); it != word_list.end(); it++){  
                        if((p = (struct Param*)malloc(sizeof(*p))) == NULL){
                            fprintf(stderr,"MALLOC THREAD_PARAM ERROR");
                            return (-1);
                        }
                        cout << p << endl;
                        cout << "marker1-1: " << i <<endl;
                        p->input = buf;
                        cout << "marker1-2" <<endl;
                        p->word = *it;
                        cout << "marker1-3" <<endl;
                        pointers.push_back(p);
                        

                        if(pthread_create(&threads[i++], 0, kmp, (void*)p) < 0 ) {
                            printf("pthread_create error\n");
                            return 0;
                        }
                    }
                    cout << "marker2" << endl;

                    int * ret = new int[word_list.size()];
                    for(i = 0; i < word_list.size(); i++){
                        pthread_join(threads[i], (void**)&ret[i]);
                    }

                    cout << "marker3" << endl;

                    vector<pair<string,int> > v(result.begin(), result.end());
                    sort(v.begin(), v.end(), compare_pair_second<less>());

                    vector<pair<string, int> >::iterator it = v.begin();
                    for(int cnt = v.size(); cnt != 0; cnt--, it++){
                        if(it->second != -1) {
                            cout << it->first;
                            if(cnt != 1) {
                                cout << "|";
                            }
                        }
                    }
                    cout << std::endl;
                    result.clear();
                    delete[] ret;
                    delete[] threads;
                    for (vector<struct Param* >::iterator it = pointers.begin() ; it != pointers.end(); ++it)
                    {
                        cout << *it << endl;
                        free (*it);
                    } 
                    pointers.clear();
 
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
    int m = (int)p.size(), j=0; 
    vector<int> pi(m, 0); 
    for(int i = 1; i< m ; i++){ 
        while(j > 0 && p[i] != p[j]) 
            j = pi[j-1]; 
        if(p[i] == p[j]) 
            pi[i] = ++j; 
    } 
        return pi; 
} 

void* kmp(void * param){
    int *ret = (int*)malloc(sizeof(int));
    struct Param *readParams = (struct Param*)param;
    string s = readParams->input;
    string p = readParams->word;
    //cout << "s: " << s << " p: " << p << endl;
    auto pi = getPi(p); 
    int n = (int)s.size(), m = (int)p.size(), j =0; 
    for(int i = 0 ; i < n ; i++){ 
        while(j>0 && s[i] != p[j]) 
            j = pi[j-1]; 
        if(s[i] == p[j]){ 
            if(j==m-1){
                *ret = i-m+1;
                result[p] = i-m+1;
                return ret;
            } else { 
                j++; 
            } 
        } 
    } 
    *ret = -1;
    result[p] = -1;
    return ret;
}