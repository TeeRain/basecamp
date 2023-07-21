CC=g++
CFLAGS=-c -Wall
LDFLAGS=
LIBS=-lpthread

all: app

app: main.o file_search.o
	$(CC) $(LDFLAGS) -o app main.o file_search.o $(LIBS)

main.o: main.cpp file_search.h
	$(CC) $(CFLAGS) main.cpp

file_search.o: file_search.cpp file_search.h
	$(CC) $(CFLAGS) file_search.cpp

clean:
	rm -rf *.o app
