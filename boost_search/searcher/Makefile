FLAG=-I ~/ThirdPartLibForCpp/el7.x86_64/third_part/include -std=c++11 -ljsoncpp

.PHONY:all
all:searcher_test libsearcher.so 

searcher_test:searcher_test.cc searcher.cc
	g++ -o $@ $^ $(FLAG)

libsearcher.so:searcher.cc searcher.h
	g++ searcher.cc -o libsearcher.so -shared -fPIC $(FLAG)

.PHONY:clean
clean:
	rm searcher_test libsearcher.so
