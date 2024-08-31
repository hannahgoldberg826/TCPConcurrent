# A very dumb Makefile

all:	server client

server:
	gcc -o tcp-server-process server-tcp-process.c AddressUtility.c \
		DieWithMessage.c TCPServerUtility.c TCPMsgUtility.c
	gcc -o tcp-server-thread server-tcp-thread.c AddressUtility.c \
		DieWithMessage.c TCPServerUtility.c TCPMsgUtility.c -pthread

client:
	gcc -o tcp-client-delayed client-tcp-delayed.c AddressUtility.c \
		DieWithMessage.c TCPClientUtility.c TCPMsgUtility.c

clean:
	rm -f *.o tcp-server tcp-client

