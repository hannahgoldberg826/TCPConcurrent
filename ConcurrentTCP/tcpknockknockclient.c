/*
 * client-tcp-delayed.c
 *
 *  Version 2.1
 *
 * This is still the same TCP client example like the previous
 * one except that we have added some artificial delays
 * to test the concurrent server. Without these delays, for this
 * overly-simple client/server, it is not very likely to get
 * multiple clients connected to the server simultaneously.
 *
 * A very simple TCP client that reads a line of user input
 * and sends it (message) to a TCP server. The client then
 * waits for the server to acknowledge the receipt of the
 * message and terminates.
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       // time()
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char **argv)
{
    int sock;
    char *server;
    char *servPort;     // either a decimal port number in string form
                        // or a standard service name

    if (argc != 3) {
        fprintf(stderr, "Usage: %s server port/service\n", argv[0]);
        exit(0);
    }

    server = argv[1];     // First arg: server address/name
    servPort = argv[2];   // Second arg: server port/service

    // seed the random number generator with a "random" number
    srand(time(NULL));

    // Create a connected TCP socket
    sock = SetupTCPClientSocket(server, servPort);
    if (sock < 0)
      DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");

   // Set the first message to "Knock Knock"
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    strncpy(buf, "Knock Knock", BUFSIZE - 1);
    
    //just for testing take this out later 
    int seconds = 3 + rand() % 8;
    printf("Sleeping for %d seconds ...\n", seconds);
    sleep(seconds);

    // Send the first message to the server
    SendMsg(sock, buf, strlen(buf));

    // ... again, DO NOT do this random delay in production code!!
    seconds = 5 + rand() % 7;
    printf("Sleeping again for %d seconds ...\n", seconds);
    sleep(seconds);
    
    // Receive the ack message for the first message
    int msglen = RecvMsg(sock, buf, BUFSIZE);
    if (msglen > BUFSIZE)
        DieWithUserMessage("RecvMsg()", "message too long to fit in the buffer");

    // Display the received ack message for the first message
    fputs("Received ack for the first message: ", stdout);
    fflush(stdout);
    write(1, buf, msglen);
    fputc('\n', stdout);

    // Set the second message to "Robin"
    memset(buf, 0, BUFSIZE);
    strncpy(buf, "Robin", BUFSIZE - 1);
    
    //just for testing take this out later 
    seconds = 3 + rand() % 7;
    printf("Sleeping for %d seconds ...\n", seconds);
    sleep(seconds);

    // Send the second message to the server
    SendMsg(sock, buf, strlen(buf));


    // ... again, DO NOT do this random delay in production code!!
    seconds = 5 + rand() % 8;
    printf("Sleeping again for %d seconds ...\n", seconds);
    sleep(seconds);
    
    // Receive the ack message for the second message
    msglen = RecvMsg(sock, buf, BUFSIZE);
    if (msglen > BUFSIZE)
        DieWithUserMessage("RecvMsg()", "message too long to fit in the buffer");

    // Display the received ack message for the second message
    fputs("Received ack for the second message: ", stdout);
    fflush(stdout);
    write(1, buf, msglen);
    fputc('\n', stdout);

    // Set the third message to "Robin you, hand over the cash!"
    memset(buf, 0, BUFSIZE);
    strncpy(buf, "Robin you! Hand over the cash!", BUFSIZE - 1);

    //just for testing take this out later 
    seconds = 3 + rand() % 8;
    printf("Sleeping for %d seconds ...\n", seconds);
    sleep(seconds);
    
    // Send the third message to the server
    SendMsg(sock, buf, strlen(buf));

    // Receive the ack message for the third message
    msglen = RecvMsg(sock, buf, BUFSIZE);
    if (msglen > BUFSIZE)
        DieWithUserMessage("RecvMsg()", "message too long to fit in the buffer");
    // close sock to release resource
    close(sock);

    return 0;
}