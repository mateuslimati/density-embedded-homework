# Density Linux C++ System Engineer Homework
Homework for Embedded and Linux System Engineer Candidates

## Goal

Our goal is to understand your knowledge of:

- Modern C++ (Especially resource management and object oriented design)
- Linux System programming
- Linux service management (systemd services)
- Linux development/build tools


## Assignment

Your task is to implement a Linux service that maintains a single current count and modifies the count by accepting commands over TCP/IP socket.

### Service Requirements

* Listen for count commands on TCP/IP socket port 8089.
* Command structures are ascii strings followed by CRLF
  * Example: INCR 5\r\n
* The following counting commands are required:
  * 'INCR <integer>' : Increment the current count by integer amount
  * 'DECR <integer>' : Decrement the current count by integer amount
  * 'OUTPUT' : Write the current count as an ascii string to the socket which requested it.
* For every count change, the current count should be written to each active connection.
* If the service receives a SIGTERM, it shall shutdown all of it's connections and exit with a 0 status code.
* Do NOT use 3rd party libraries like boost for this assignment
* The service shall be single threaded and support up to 1024 connections.  (hint use epoll or select for asyncio).

### Systemd Requirements

* *Explain* (no need to implement) how this service could be integrated with systemd so that it will:
  * Be restarted after a crash
  * All output to stderr will be sent to the journal

### Building

Please provide a Makefile or CMakeLists.txt file that will build the service as the default target.

### Testing
telnet or socat can be used for sending commands and receiving count updates.

    telnet localhost 8089 

## Don't Let the Dog Eat Your Homework

Please create a separate github repository to implement this homework assignment and then send us a link when you are ready for us to review.

## Notes

- Please do not fork or submit a PR to this repo
- Please document your thought-processes and use well-written git commit messages to show your progress
- This assignment should take less than 5 hours to complete
- If you get stuck or need more information, please reach out for clarity
- Have fun!
