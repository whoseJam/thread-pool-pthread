
run: test.o thpool.o
	g++ test.o thpool.o -o run -g -lpthread

test.o:test.cpp
	g++ -c test.cpp -g

thpool.o:thpool.cpp
	g++ -c thpool.cpp -g

clean:
	rm *.o