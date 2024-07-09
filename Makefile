
all: main

main: main.cc
	g++ -o a1 main.cc -l sqlite3

clean:
	rm -rf *o all a1 library.db
