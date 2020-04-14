lib=libpipi.a

$(lib):add.o sub.o
	ar -rc $@ $^
add.o:add.c
	gcc -c add.c
sub.o:sub.c
	gcc -c sub.c
.PHONY:output
output:
	mkdir -p lib/include
	mkdir -p lib/lib
	cp *.a lib/lib 
	cp *.h lib/include
.PHONY:clean
clean:
	rm -rf lib *.a *.o
