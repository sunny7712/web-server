# Simple Web Server in C

## Overview
A simple web server implementation in C following the HTTP/1.1 specification. This project is part of a coding challenge to better understand web servers and network programming.  
Challenge details: [codingchallenges.fyi/challenges/challenge-webserver](https://codingchallenges.fyi/challenges/challenge-webserver/)

## Features
- Basic HTTP/1.1 server
- Handles GET requests
- Serves static files from a configurable directory
- Simple error handling (404 Not Found, 400 Bad Request, etc.)
- Minimal logging to stdout

## Requirements
- GCC or compatible C compiler
- POSIX-compliant system (Linux, WSL, macOS)
- `make` utility

## Building

```bash
make
```

This will produce an executable named `web_server`. Runs on port 4221.

## Running

```bash
./web_server 
```

Example:

```bash
./web_server
```

## Progress

- [x] Basic server setup
- [x] Handle GET requests
- [x] Serve static files
- [x] Simple error handling
- [ ] Handling concurrent requests (future work)
