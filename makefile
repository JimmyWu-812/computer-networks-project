CC = g++
CVERSION = -std=c++17
CFLAGS = -c

all	: server client
	@echo -n ""

server			: server.o
			$(CC) $< -o $@
client			: client.o
			$(CC) $< -o $@
server.o 	   	: server.cpp util.cpp
			$(CC) $(CVERSION) $(CFLAGS) $< -o $@
client.o 	   	: client.cpp util.cpp
			$(CC) $(CVERSION) $(CFLAGS) $< -o $@

clean_all:
		rm -rf *.o server client client_dir server_dir
clean:
		rm -rf *.o server client