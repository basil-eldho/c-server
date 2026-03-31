# Building a Server from Scratch

Learning systems programming by writing a real HTTP server in C — no frameworks, no abstractions, just syscalls and raw sockets.

The end goal is a production-grade server on par with Go's `net/http`: concurrent by default, TLS, HTTP/2, proper timeouts, graceful shutdown, and a clean handler API. We build up to that one layer at a time.

---

## Why?

The best way to understand how the internet works is to build the thing yourself. By the end of this, you'll know exactly what happens between a browser typing a URL and a webpage showing up — because you wrote every layer of it.

Go's standard library server is a great benchmark. It's not magic — it's the same TCP sockets, the same HTTP parsing, the same TLS handshake. We're going to build that from scratch and understand every bit of it.

---

## The Plan

### Phase 1 — TCP Echo Server
> *"Can two programs talk to each other?"*

Before HTTP, before files, before any of that — just open a socket, listen for a connection, and echo back whatever the client sends.

**You'll learn:** `socket`, `bind`, `listen`, `accept`, `read`, `write`

---

### Phase 2 — HTTP Request Parser
> *"What is the browser actually saying?"*

HTTP is just text over a TCP connection. You'll read the raw bytes and parse them into something structured — method, path, headers, body. No library. You write the parser.

**You'll learn:** HTTP/1.1 request format, string parsing in C, reading until `\r\n\r\n`

---

### Phase 3 — HTTP Response Sender
> *"Talking back to the browser."*

Build response structs, write status lines, set headers, send a body. Point your browser at your server and see a real webpage load.

**You'll learn:** HTTP response format, status codes, `Content-Type`, `Content-Length`

---

### Phase 4 — Static File Server
> *"Serving real files off disk."*

Read files from the filesystem and serve them over HTTP. Handle different file types. Return 404s for missing files. At this point you have a real, working web server.

**You'll learn:** `open`, `read`, `stat`, MIME types, directory traversal safety

---

### Phase 5 — Concurrency
> *"Handling more than one person at a time."*

Right now the server handles one connection, then the next. We'll fix that — first with `fork`, then `pthreads`, then `epoll` (event-driven I/O — how Nginx actually works and how Go's runtime schedules goroutines under the hood).

**You'll learn:** `fork`, `pthreads`, `epoll`, blocking vs non-blocking I/O, thread pools

---

### Phase 6 — Robustness & Timeouts
> *"Making it not fall over."*

Real servers handle slow clients, time out gracefully, and don't crash on bad input. This phase adds the safety layer around everything built so far.

**You'll learn:** Keep-alive connections, read/write/idle timeouts, signal handling, `SIGPIPE`, graceful shutdown

---

### Phase 7 — Router & Handler API
> *"A clean interface for building on top of."*

Go's `net/http` gives you `http.HandleFunc("/path", handler)`. We'll build the same thing — a router that matches paths and dispatches to handlers. This is where the server becomes a framework.

**You'll learn:** Trie/map-based routing, function pointers in C, middleware chains

---

### Phase 8 — TLS
> *"HTTPS. The S stands for Sockets."*

Wrap the TCP layer with TLS using OpenSSL or a lightweight alternative. Understand the TLS handshake, certificates, and why HTTPS is just HTTP with a security layer underneath.

**You'll learn:** TLS handshake, `SSL_read`/`SSL_write`, certificates, cipher suites

---

### Phase 9 — HTTP/2
> *"The modern web."*

HTTP/2 is binary, multiplexed, and header-compressed. This is the hardest phase — you'll implement frame parsing, stream multiplexing, and HPACK header compression.

**You'll learn:** HTTP/2 framing, HPACK, stream multiplexing, binary protocols

---

### Phase 10 — Production Hardening
> *"Ship it."*

Logging, metrics, connection limits, backpressure, zero-downtime reload, and configuration. Everything that separates a toy from something you'd actually run.

**You'll learn:** Structured logging, `SO_REUSEPORT`, worker processes, config files, observability

---

## End goal

A server you could actually run in production — comparable to what Go's `net/http` gives you out of the box, but written by you, in C, with full understanding of every layer underneath it.

---

## Structure

```
server/
  phase1/    TCP echo server
  phase2/    HTTP parser
  phase3/    HTTP responses
  phase4/    Static files
  phase5/    Concurrency
  phase6/    Robustness & timeouts
  phase7/    Router & handler API
  phase8/    TLS
  phase9/    HTTP/2
  phase10/   Production hardening
```

---

## Prerequisites

- Basic C (pointers, structs, memory allocation)
- A Linux machine (all syscalls here are POSIX/Linux)
- `gcc`, `make`, `curl`, `openssl`, `nc` for testing

You don't need to know networking. That's the point.
