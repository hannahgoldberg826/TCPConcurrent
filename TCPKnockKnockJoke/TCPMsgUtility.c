#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

/*
 * This implementation receives a message from a socket with
 * the assumption that the message has an integer length
 * marker in front of it.
 *
 * Note: if the message is too long to fit (msglen > bufsize),
 * the buffer will only contain partial message up to the point
 * its capacity will allow. Any extra bytes are not read.
 */
int RecvMsg(int sock, char *buf, int bufsize)
{
    int msglen;
    ssize_t numBytesRcvd;

    // receive the message length first
    int total_bytes_received = 0;
    char *pMsgLen = (char *)&msglen;
    while (total_bytes_received < sizeof(msglen)) {
        numBytesRcvd = recv(sock, pMsgLen+total_bytes_received,
                            sizeof(msglen)-total_bytes_received, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv(len) failed");
        total_bytes_received += numBytesRcvd;
    }
    msglen = ntohl(msglen); // convert msglen to host byte order

    // check to prevent from reading more than what is necessary
    if (msglen < bufsize)
        bufsize = msglen;

    // receive the message body
    total_bytes_received = 0;
    while (total_bytes_received < bufsize) {
        numBytesRcvd = recv(sock, buf+total_bytes_received,
                            bufsize-total_bytes_received, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv(msg) failed");
        total_bytes_received += numBytesRcvd;
    }

    return msglen;
}

/*
 * This implementation sends a message through a socket by
 * sending an integer marker first. The integer marker enables
 * the receiver to figure out when a message has been completely
 * received.
 */
void SendMsg(int sock, char *msg, int msglen)
{
    ssize_t numBytesSent;

    // send the message length first
    int netlen = htonl(msglen); // convert msglen to network byte order
    numBytesSent = send(sock, (char *)&netlen, sizeof(netlen), 0);
    if (numBytesSent < 0)
        DieWithSystemMessage("send() failed");
    else if (numBytesSent != sizeof(msglen))
        DieWithUserMessage("send(len)", "sent unexpected number of bytes");

    // send the message body
    numBytesSent = send(sock, msg, msglen, 0);
    if (numBytesSent < 0)
        DieWithSystemMessage("send() failed)");
    else if (numBytesSent != msglen)
        DieWithUserMessage("send(msg)", "sent unexpected number of bytes");
}


