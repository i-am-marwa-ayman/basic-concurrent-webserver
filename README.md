# Multi-threaded Web Server

A simple multi-threaded web server implementation that handles concurrent HTTP requests using a thread pool architecture. This project was completed as part of the Operating Systems: Three Easy Pieces (OSTEP) course to demonstrate and apply concurrency principles.

## OSTEP Course Project

This implementation is an extension of the web server project from the OSTEP curriculum. The original server was single-threaded, and this version has been enhanced to support concurrent request handling through a thread pool design, applying the concurrency concepts (producer-consumer) taught in the course.


## Building

```bash
# Compile the project using makefile
make
```

## Usage

### Starting the Server

```bash
./wserver [-d <basedir>] [-p <port>] [-t <threads>] [-b <buffers>]
```

Parameters:
- `-d <basedir>`: Root directory for serving files (default: current directory)
- `-p <port>`: Port number to listen on (default: 10000)
- `-t <threads>`: Number of worker threads (default: 1)
- `-b <buffers>`: Size of request buffer (default: 1)

Example:
```bash
./wserver -d ./test_files -p 8080 -t 4 -b 16
```

### Testing with the Client

```bash
./wclient <hostname> <port> <filename>
```

Example:
```bash
./wclient localhost 8080 /index.html
```
Example:

```bash
./wclient localhost 8080 /spin.cgi?5 # just to make it wait for n seconds
```

### Concurrent Testing

To test the server's ability to handle concurrent requests and verify thread pool functionality:

1. Start the server with a single thread:
   ```bash
   ./wserver -d ./test_files -p 8080 -t 1 -b 5
   ```

2. Send a long-running request:
   ```bash
   ./wclient localhost 8080 /spin.cgi?5
   ```

3. Immediately send a short-running request:
   ```bash
   ./wclient localhost 8080 /spin.cgi?1
   ```

4. With only one thread, the second request will complete after the first one (about 6 seconds total).

5. Restart the server with multiple threads:
   ```bash
   ./wserver -d ./test_files -p 8080 -t 2 -b 5
   ```

6. Repeat the test - now the second request should complete in about 1 second even while the first is still processing.

## Architecture

The server follows a classic producer-consumer architecture as taught in the OSTEP course:
1. The master thread acts as the producer, accepting new connections and adding them to the request buffer.
2. Worker threads act as consumers, taking requests from the buffer and processing them.
3. Synchronization mechanisms (mutexes and condition variables) ensure thread-safe access to the shared buffer.

## Learning Outcomes

Through this project, I applied several key OSTEP concepts:
- Thread creation and management
- Producer-consumer problem solution
- Bounded buffer implementation
- Mutex and condition variable usage
- Concurrent programming challenges

## Note

- The server is designed for educational purposes and not suitable for production environments.

## Future Improvements

- Make a circular queue for better buffer handling.
- Add Smallest File First (SFF) scheduling Policy.