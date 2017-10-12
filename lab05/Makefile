all : memcheck helgrind prime_cond

memcheck : memcheck_test.cpp
	g++ -g -o memcheck_test memcheck_test.cpp

helgrind : helgrind_test.cpp
	g++ -g -o helgrind_test helgrind_test.cpp -lpthread

prime_cond : prime_cond_err.cpp
	g++ -g -o prime_cond_err prime_cond_err.cpp -lpthread
