all : prac_threadgroup.cpp
	g++ -o prac_threadgroup prac_threadgroup.cpp -lboost_system -lboost_thread
	g++ -o prime_threadgroup prime_threadgroup.cpp -lboost_system -lboost_thread

prime : prime_threadgroup.cpp
	g++ -o prime_threadgroup prime_threadgroup.cpp -lboost_system -lboost_thread

clean :
	rm prac_threadgroup
	rm prime_threadgroup