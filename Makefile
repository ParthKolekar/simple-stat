override CFLAGS += -Ofast -funroll-loops -flto -Wall
override LFLAGS += 
CC = gcc

all: library syscall

clean :
	rm -f *.o simple-stat-library simple-stat-syscall

clear : clean

library: 
	$(CC) $(CFLAGS) -c simple-stat-library.c
	$(CC) $(CFLAGS) -o simple-stat-library simple-stat-library.o $(LFLAGS)

syscall:
	$(CC) $(CFLAGS) -c simple-stat-syscall.c
	$(CC) $(CFLAGS) -o simple-stat-syscall simple-stat-syscall.o $(LFLAGS)


