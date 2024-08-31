./- This package contains a simple TCP server/client program.
The TCP server is concurrent. Two variants are available:
one is based on processes while the other is based on 
threads.

After unzipping the package into a folder (directory), 
you should see these files:

	AddressUtility.c      README.txt            client-tcp-delayed.c
	DieWithMessage.c      TCPClientUtility.c    server-tcp-process.c
	Makefile              TCPMsgUtility.c       server-tcp-thread.c
	Practical.h           TCPServerUtility.c

1. Compile the code

Open a terminal window, change working folder (directory) to 
where the code is.

You can compile the package manually by typing:

	gcc -o tcp-server-process server-tcp-process.c AddressUtility.c \
		DieWithMessage.c TCPServerUtility.c TCPMsgUtility.c
	gcc -o tcp-server-thread server-tcp-thread.c AddressUtility.c \
		DieWithMessage.c TCPServerUtility.c TCPMsgUtility.c -pthread
	gcc -o tcp-client-delayed client-tcp-delayed.c AddressUtility.c \
		DieWithMessage.c TCPClientUtility.c TCPMsgUtility.c

But a more standard way is to use the make utility:

	make server
	make client

You should take a peek at the Makefile. To build both at the same time, simply:

	make

What the commands do is to create three programs:

	tcp-server-process			# process-based concurrent server
	tcp-server-thread			# process-based concurrent server
	tcp-client-delayed			# tcp client with artificial delays inserted

2. Run the code

Ideally you run the server on one machine and run the client 
on another machine.  However you can run both the server and
the client on the same machine as well (thanks to multitasking).

2.1 On two different machines

On one machine, open a terminal window and change working 
folder (directory) to where the code is.

Run the process-based server such as:

	./tcp-server-process 6789

Alternatively (not both), run the thread-based server such as:

	./tcp-server-thread 6789

This starts the server on local machine at port 6789.

On the other machine, open a terminal window and change working 
folder (directory) to where the code is.

Run the client such as:

	./tcp-client-delayed <server's ip address> 6789

This starts the client to call the remote server at port 6789.

Follow the prompt and type a message to be sent to the server.

2.2 On the same machine

Open two terminal windows and change working folders
to where the code is.

Run the process-based server such as:

	./tcp-server-process 6789

Alternatively (not both), run the thread-based server such as:

	./tcp-server-thread 6789

This starts the server on local machine at port 6789.

Run the client in the other terminal window such as:

	./tcp-client-delayed localhost 6789

This starts the client to call the server running on local
machine at port 6789. The "localhost" is a special name 
that refers to the local machine.

Follow the prompt and type a message to be sent to the server.
