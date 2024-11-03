all: number-server

number-server: number-server.c http-server.c
	gcc -std=c11 number-server.c http-server.c -o number-server -Wall -Wno-unused-variable -fsanitize=address -g

clean:
	rm -f number-server
