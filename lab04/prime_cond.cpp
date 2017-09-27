#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <iostream>

#define NUM_THREAD  10

int thread_ret[NUM_THREAD];

int range_start;
int range_end;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

bool is_done = false; //help

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }

    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void* ThreadFunc(void* arg) {

    long tid = (long)arg;

    while(true) { 
    pthread_mutex_lock(&lock);
    thread_ret[tid] = -1;
    pthread_cond_wait(&cond, &lock);
    pthread_mutex_unlock(&lock);

    // Split range for this thread
    int start = range_start + ((range_end - range_start) / NUM_THREAD) * tid;
    int end = range_start + ((range_end - range_start) / NUM_THREAD) * (tid+1);
    if (tid == NUM_THREAD - 1) {
        end = range_end + 1;
    }
    
    long cnt_prime = 0;
    for (int i = start; i < end; i++) {
        if (IsPrime(i)) {
            cnt_prime++;
        }
    }

    pthread_mutex_lock(&lock);
    thread_ret[tid] = cnt_prime;   
    pthread_cond_wait(&cond, &lock); //when condition variable calls, should have mutex

    pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREAD];


    // Create threads to work
    for (long i = 0; i < NUM_THREAD; i++) {
        if (pthread_create(&threads[i], 0, ThreadFunc, (void*)i) < 0) {
            printf("pthread_create error!\n");
            return 0;
        }
    } // ok
    

    while (1) {
        // Input range
        scanf("%d", &range_start);
        if (range_start == -1) {
            is_done = true;
            pthread_mutex_lock(&lock); 
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            break;
        }
        scanf("%d", &range_end);

        /*for (int i = 0; i < NUM_THREAD; i++) {
            thread_ret[i] = -1;
        }*/

        // Wait threads end
        
        pthread_mutex_lock(&lock); 
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);

        while(1){
            bool all_thread_done = true;
            for(int i = 0; i<NUM_THREAD; i++) {
                if(thread_ret[i] < 0) {
                    all_thread_done =false;
                    break;
                } 
            }
            if(all_thread_done) {
                break;
            }
            //pthread_yield();
        }

        // Collect results
        int cnt_prime = 0;
        for (int i = 0; i < NUM_THREAD; i++) {
            cnt_prime += thread_ret[i];
        }
        printf("number of prime: %d\n", cnt_prime);
    }

    return 0;
}

