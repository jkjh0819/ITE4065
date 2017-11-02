#include <stdio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#define NUM_THREAD_IN_POOL 8

boost::asio::io_service print_io;
boost::thread * print;
boost::asio::io_service::work * print_work;

void Print(int result, int start, int end, int sequence){ 
    printf("(%d)number of primes in %d ~ %d is %d\n", sequence, start, end, result);
}

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

void* ThreadFunc(int start, int end, int sequence) {
 
    long cnt_prime = 0;
    for (int i = start; i < end; i++) {
        if (IsPrime(i)) {
            cnt_prime++;
        }
    }

    print_io.post(boost::bind(&Print, cnt_prime, start, end, sequence));

}

int main(void) {
    boost::asio::io_service io;
    boost::thread_group threadpool;
    boost::asio::io_service::work * work = new boost::asio::io_service::work(io);

    print_work = new boost::asio::io_service::work(print_io);
    print = new boost::thread(boost::bind(&boost::asio::io_service::run, &print_io));

    for(int i = 0; i < NUM_THREAD_IN_POOL; i++){
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &io));
    }

    int sequence = 0;

    int range_start = 0;
    int range_end = 0;

    while(1) {
        scanf("%d", &range_start);
        
        if(range_start == -1){
            std::cout << "finish test." << std::endl;
            break;
        }

        scanf("%d", &range_end);

        io.post(boost::bind(&ThreadFunc, range_start, range_end, sequence));
        sequence++;

    }

    delete work;
    threadpool.join_all();
    
    delete print_work; 
    print->join();

    return 0;

}

