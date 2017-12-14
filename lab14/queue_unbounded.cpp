#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

#define NUM_PRODUCER                1
#define NUM_CONSUMER                NUM_PRODUCER
#define NUM_THREADS                 (NUM_PRODUCER + NUM_CONSUMER)
#define NUM_ENQUEUE_PER_PRODUCER    1000
#define NUM_DEQUEUE_PER_CONSUMER    NUM_ENQUEUE_PER_PRODUCER

void enqueue(int key);
int dequeue();

struct QueueNode {
    int key;
    QueueNode * next;
};

QueueNode* head;
QueueNode* tail;

void init_queue(void) {
    QueueNode* centinel = (QueueNode*)malloc(sizeof(QueueNode));
    centinel->next = NULL;
    head->next = centinel;
    tail->next = centinel; 
}

void enqueue(int key) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->key = key;
    newNode->next = NULL;
    
    // first way
    // QueueNode* old_tail = __sync_lock_test_and_set(&tail->next, newNode);
    // old_tail->next = newNode;

    //second way
    while(__sync_bool_compare_and_swap(&tail->next, NULL, new_node) == false){
        pthread_yield_np();
    }
    tail = new_node;

    //third way
    QueueNode* tail_copied;
    while(1){
        tail_copied = tail;
        QueueNode* ret_node = __sync_val_compare_and_swap(&tail_copied->next, NULL, new_node);

        if(ret_node == NULL){
            break;
        } else {
            __sync_bool_compare_and_swap(&tail, tail_copied, ret_node);
        }
    }
    if(tail == tail_copied)
        __sync_bool_compare_and_swap(&tail, tail_copied, new_node);
}

int dequeue(void) {
    QueueNode* centinel;
    QueueNode* dequeue_target;
    int ret_key;
    while(true){
        /*
        centinel = head->next;
        dequeue_target = centinel->next;
        if(dequeue_target != NULL){
            //printf("%p", dequeue_target);
            if(__sync_bool_compare_and_swap(&centinel->next, dequeue_target, dequeue_target->next)){
                return dequeue_target->key;
            }
        }*/
        centinel = head; 
        if(centinel->next != NULL){
            if(__sync_bool_compare_and_swap(&head, centinel, centinel->next)){
                ret_key = centinel->next->key;
                break;
            }
        } else {
            pthread_yield_np();
        }
    }

    return ret_key;
}

void* ProducerFunc(void* arg) {
    long tid = (long)arg;

    int key_enqueue = NUM_ENQUEUE_PER_PRODUCER * tid;
    for (int i = 0; i < NUM_ENQUEUE_PER_PRODUCER; i++) {
        //printf("enqueue : %d\n", key_enqueue);
        enqueue(key_enqueue);
        key_enqueue++;
    }

    return NULL;
}

void* ConsumerFunc(void* arg) {
    for (int i = 0; i < NUM_DEQUEUE_PER_CONSUMER; i++) {
        int key_dequeue = dequeue();
        //printf("dequeue : %d\n", key_dequeue);
    }

    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];

    head = (QueueNode*)malloc(sizeof(QueueNode));
    tail = (QueueNode*)malloc(sizeof(QueueNode));

    init_queue();

    for (int i = 0; i < NUM_THREADS; i++) {
        if (i < NUM_PRODUCER) {
            pthread_create(&threads[i], 0, ProducerFunc, (void**)i);
        } else {
            pthread_create(&threads[i], 0, ConsumerFunc, NULL);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

