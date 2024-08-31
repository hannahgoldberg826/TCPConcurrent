# A very dumb Makefile

all:	server client

server:
	
	gcc -o tcpknockknockserver tcpknockknockserver.c AddressUtility.c \
		DieWithMessage.c TCPServerUtility.c TCPMsgUtility.c -pthread

client:
	gcc -o tcpknockknockclient tcpknockknockclient.c AddressUtility.c \
		DieWithMessage.c TCPClientUtility.c TCPMsgUtility.c

clean:
	rm -f *.o tcp-server tcp-client

