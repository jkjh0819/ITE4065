CC=g++
CFLAGS=-g -O3 --std=c++11
OBJECTS = ref_solution.o ahocorasick.o

run: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o run
