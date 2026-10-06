all: diyis client

diyis: main.o
	g++ -Wall -Wextra main.o -o diyis-server

client: client.o
	g++ -Wall -Wextra client.o -o diyis-client

main.o: main.cc
	g++ -Wall -Wextra -c main.cc

client.o: client.cc
	g++ -Wall -Wextra -c client.cc

clean:
	rm -f *.o diyis-*
