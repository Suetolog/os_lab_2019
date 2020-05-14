CC=gcc
CFLAGS= -std=c99 -I .

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c  $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o  -c utils.c  $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c  $(CFLAGS)

process_memory : process_memory.c
	$(CC) -o procmem process_memory.c 

clean :
	rm utils.o find_min_max.o parallel_min_max  procmem paralsum

all : parallel_min_max process_memory paralsum

paralsum : parallel_sum.c utils.o
	$(CC) -o paralsum utils.o parallel_sum.c -g -lpthread $(CFLAGS)
    
	 
