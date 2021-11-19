# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
# AR = ar
# ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
# DBGFLAGS = -g -D_DEBUG_ON_
# OPTFLAGS = -O3

all	: server
	@echo -n ""

# optimized version
server			: server.o
			$(CC) $< -o $@
client			: client.o
			$(CC) $< -o $@
server.o 	   	: server.cpp
			$(CC) $(CFLAGS) $< -o $@
client.o 	   	: client.cpp
			$(CC) $(CFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o server client