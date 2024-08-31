This package includes a simple concurrent TCP server and client program, with two server variants: one using processes and the other using threads.

**Files Included:**
- AddressUtility.c
- README.txt
- client-tcp-delayed.c
- DieWithMessage.c
- TCPClientUtility.c
- server-tcp-process.c
- Makefile
- TCPMsgUtility.c
- server-tcp-thread.c
- Practical.h
- TCPServerUtility.c

**Steps to Compile:**

1. **Manual Compilation:**
   - Use `gcc` to compile the server and client programs.

2. **Using Makefile:**
   - Run `make server` to build the servers.
   - Run `make client` to build the client.
   - Or simply use `make` to build all programs at once.

**Programs Created:**
- `tcp-server-process`: Process-based concurrent server.
- `tcp-server-thread`: Thread-based concurrent server.
- `tcp-client-delayed`: TCP client with artificial delays.

**Steps to Run:**

1. **On Two Machines:**
   - Run the server on one machine and the client on another, using the server's IP address and port number.
  
2. **On the Same Machine:**
   - Open two terminal windows, run the server in one, and the client in the other using "localhost" as the address.

Follow the prompts to send messages from the client to the server.
