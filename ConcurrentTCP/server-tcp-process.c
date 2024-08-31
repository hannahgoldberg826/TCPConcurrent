/*
 * server-tcp-process.c
 *
 *  Version 2.1
 *
 * This is a concurrent version of the TCP server for the
 * previous TCP example (with markers also). This version
 * uses processes for multitasking. For the version that
 * uses threads, see server-tcp-thread.c
 *
 * This is a very simple TCP server that waits for a connection
 * from a client. Once the client is connected, the server
 * forks a new child process to deal with the client while
 * the main (parent) server process goes back to wait for another
 * new connection from a new client. As the parent server process
 * accepts more connections, more child server processes will
 * be created resulting in many processes serving many clients
 * simultaneously.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>       // for waitpid()
#include "Practical.h"

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

    unsigned int childProcCount = 0; // Number of child processes
    while (true) {  // Run forever
        // New connection creates a connected client socket
        int clntSock = AcceptTCPConnection(servSock);

        // ready to fork a new process to take care of the newly connected
        // client so that the old process can go back and accept new
        // connections from more clients

        // Fork child process and report any errors
        pid_t processID = fork();
        // At this point, we have two processes: the old (parent) process
        // and the new (child) process.
        // They go by running exactly the same code that follows but
        // with different process contexts -- the new process duplicates
        // its process context from its parent.
        // That means they both will have their OWN COPY of "processID"
        // variable along with others.
        // They both even have their OWN COPIES of the (same) sockets!!
        if (processID < 0)
            DieWithSystemMessage("fork() failed");

        // Since most of the time we don't want the parent and the child to do
        // exactly the same thing by running exactly the same code, we will use
        // an if-statement starting at this point to:
        //    -- execute one branch of the code if it is parent
        //    -- execute the other branch of the code if it is child
        if (processID == 0) { // If this is run by the child process
            close(servSock);         // Child closes its copy of parent socket
            MyHandleTCPClient(clntSock);
            // close client socket
            close(clntSock);
            exit(0);                 // Child process terminates at this point
        }

        // below is running by parent process because child process cannot get here
        printf("with child process: %d\n", processID);
        close(clntSock);  // Parent closes its copy of child socket descriptor
        childProcCount++; // Increment number of child processes

        while (childProcCount > 0) { // Clean up all zombies
            processID = waitpid((pid_t) - 1, NULL, WNOHANG); // Non-blocking wait
            if (processID < 0) // waitpid() error?
                DieWithSystemMessage("waitpid() failed");
            else if (processID == 0) // No zombie to wait on
                break;
            else
                childProcCount--; // Cleaned up after a child (zombie)
        }
    }
    // WILL NEVER REACH THIS POINT
    close(servSock);

    return 0;   // not really needed, but just in case the compiler is not happy
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
