all: encrypt brute

encrypt: encrypt.o
	g++ -std=c++11 encrypt.o -o encrypt

encrypt.o:
	g++ -std=c++11 -c encrypt.cpp

brute: brute.o
	g++ -std=c++11 brute.o -o brute

brute.o:
	g++ -std=c++11 -c brute.cpp

clean:
	rm *.o encrypt brute
