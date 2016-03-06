CC = gcc
INCLUDES = -I/usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/include
LIBS = -L/usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib  -lmysqlclient -lm -lz

all: myclient

main.o: main.c 
	$(CC)   -c  $(INCLUDES) main.c

myclient: main.o
	$(CC) -o myclient main.o  $(LIBS)

clean:
	rm -f myclient main.o