#ifndef THREADPOOL
#define THREADPOOL

#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <future>
#include <stdexcept>

using namespace std;

class ThreadPool {
public:
	ThreadPool(int num);
	~ThreadPool();

	template<class T, class... Args>
	auto add(T&& t, Args&&... args)
		->future<typename result_of<T(Args...)>::type>;

private:
	vector<thread> threads;
	queue<function<void()> > job_queue;

	condition_variable cond;
	mutex mtx;
	bool stop;
};


inline ThreadPool::ThreadPool(int num): stop(false) {
	for (int i = 0; i < num; i++) {
		threads.emplace_back([this] {
				while(true) {
					function<void()> job;
					{
						unique_lock< mutex> lock(this->mtx);
						this->cond.wait(lock, [this]{ return this->stop || !this->job_queue.empty();});

						if (this->stop && this->job_queue.empty())
							return;

						job = move(this->job_queue.front());
						this->job_queue.pop();
					}

					job();
				}
			}
		);
	}
}

inline ThreadPool::~ThreadPool() {
	{
		unique_lock<mutex> lock(mtx);
		stop = true;
	}
	cond.notify_all();

	for (thread &t : threads)
		t.join();
}

template<class T, class... Args>
auto ThreadPool::add(T&& f, Args&&... args) 
->  future<typename result_of<T(Args...)>::type> {
	using return_type = typename result_of<T(Args...)>::type;

	auto job = make_shared<packaged_task<return_type()> >(bind(forward<T>(f), forward<Args>(args)...));

	future<return_type> ret = job->get_future();
	{
		unique_lock< mutex> lock(mtx);
		job_queue.emplace([job](){ (*job)(); });
	}
	cond.notify_one();
	return ret;
}

#endif