.PHONY:all
all:shm read_shm 

shm:shm.cc
	g++ -o $@ $^

read_shm:read_shm.cc
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f shm read_shm
