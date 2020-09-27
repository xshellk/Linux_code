.PHONY:all
all:server client

server:server.cc
	g++ -o $@ $^ -std=c++11 -lpthread

client:client.cc 
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm server client
