all:
	gcc -Wall server.c -c
	gcc -Wall client.c -c
	gcc server.o -o host
	gcc client.o -o game
clean:
	rm *.o
	rm host
	rm game
