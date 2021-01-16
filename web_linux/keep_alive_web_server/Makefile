.PHONY:all
all:HttpServer testcgi

testcgi:testcgi.cc
	g++ -o $@ $^ -std=c++11 

HttpServer:main.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:
clean:
	rm -f HttpServer ./wwwroot/mkcgi/testcgi log.config


.PHONY:
output:
	mv testcgi ./wwwroot/mkcgi/
