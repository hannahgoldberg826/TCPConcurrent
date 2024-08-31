/*
 * server-tcp-thread.c
 *
 *  Version 2.1
 *
 * This is a concurrent version of the TCP server for the
 * previous TCP example (with markers also). This version
 * uses threads for multitasking. For the version that
 * uses processes, see server-tcp-process.c
 *
 * This is a very simple TCP server that waits for a connection
 * from a client. Once the client is connected, the server
 * spawns a new thread to handle the client while the main thread
 * goes back to wait for a new connection from a new client. As
 * the main server thread accepts more connections, more threads
 * will be created resulting in many threads serving many clients
 * simultaneously.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "Practical.h"

// A thread needs a piece of code called thread function to run
// It has a peculiar prototype -- read the topic of "function pointer"
// in a C book.
void *ThreadMain(void *arg); // Main program of a thread

// See the above prototype -- the thread main code needs an argument
// of type "void *" -- that is a wildcard pointer type.
// We will pass the following structure using the wildcard pointer.
//
// Structure of arguments to pass to client thread
struct ThreadArgs {
    int clntSock; // Socket descriptor for client
};

void MyHandleTCPClient(int clntSocket);

int main(int argc, char *argv[]) {
    int servSock;
    char *servPort;     // either a decimal port number in string form
                        // or a standard service name

    if (argc != 2) { // Test for correct number of arguments
        fprintf(stderr, "Usage: %s port/service\n", argv[0]);
        exit(0);
    }

    servPort = argv[1];  // first command line argument is local port/service

    // Create socket for incoming connections
    servSock = SetupTCPServerSocket(servPort);
    if (servSock < 0)
        DieWithUserMessage("SetupTCPServerSocket() failed", servPort);

    while (true) {  // Run forever
        // New connection creates a connected client socket
        int clntSock = AcceptTCPConnection(servSock);

        // Create separate memory for client argument
        struct ThreadArgs *threadArgs =
                (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
        if (threadArgs == NULL)
            DieWithSystemMessage("malloc() failed");
        threadArgs->clntSock = clntSock;

        // Create client thread
        pthread_t threadID;
        int returnValue = pthread_create(&threadID, NULL, ThreadMain, threadArgs);
        if (returnValue != 0)
            DieWithUserMessage("pthread_create() failed", strerror(returnValue));
        printf("with thread %ld\n", (long int) threadID);

        // At this point, the main thread loops back to accept new connections
        // while the newly created client thread handles the client by running
        // the thread main function.

    }
    // WILL NEVER REACH THIS POINT
    close(servSock);

    return 0;   // not really needed, but just in case the compiler is not happy
}

void *ThreadMain(void *threadArgs) {
    // detach the current thread from others so that its
    // resource will be returned to system immediately after it
    // terminates
    //
    // Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());
    // Otherwise the main thread has to "harvest" it

    // Extract socket file descriptor from argument
    int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
    free(threadArgs); // Deallocate memory for argument

    MyHandleTCPClient(clntSock);
    // close client socket
    close(clntSock);

    return (NULL);
}

void MyHandleTCPClient(int clntSocket) {
    // Block until receive message from a client
    char buffer[BUFSIZE]; // I/O buffer
    int msglen = RecvMsg(clntSocket, buffer, BUFSIZE);
    if (msglen > BUFSIZE)
        DieWithUserMessage("RecvMsg()", "message too long to fit in the buffer");

    // display the received message
    fputs("Received a message: ", stdout);
    fflush(stdout);
    write(1, buffer, msglen);
    fputc('\n', stdout);

    // acknowledge the receipt of the message from client
    char ack_msg[2*BUFSIZE];
    strcpy(ack_msg, "Got your message: ");
    buffer[msglen] = '\0';      // may go out of bound -- ignore
    strcat(ack_msg, buffer);
    SendMsg(clntSocket, ack_msg, strlen(ack_msg));
}
