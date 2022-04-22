# Chapter 4: Networked Concurrency

## Code


Function | Description 
---------|----------
 `int socket(int domain, int type, int protocol)` | Create a socket instance
 -- | --
 `uint32_t htonl(uint32_t hostlong)` | Convert a 32-bit unsigned integer from host endian format to network endian format (host-to-network-long)
 `uint16_t htons(uint16_t hostshort)` | Convert a 16-bit unsigned integer from host endian format to network endian format (host-to-network-short)
 `uint32_t ntohl(uint32_t netlong)` | Convert a 32-bit unsigned integer from network endian format to host endian format (net-to-host-long)
 `uint16_t ntohs(uint16_t netlong)` | Convert a 16-bit unsigned integer from network endian format to host endian format
 -- | --
 `int getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res)` | Translate a human-readable hostname into an IP address, typically with the help of DNS
 `void freeaddrinfo(struct addrinfo *ai)` | Free all address information structures in the linked list beginning at ai
 -- | --
 `char *inet_ntoa(struct in_addr in)` | Convert an IPv4 address into a string using dotted decimal notation
 `const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)` | Convert an IP address (either IPv4 or IPv6) into a string format
 A3 | B3
 A3 | B3
 A3 | B3
 A3 | B3 

 Code listing illustrates how these functions can be used along with `getaddrinfo()` to translate a hostname (assumed to be declared with a value such as `"www.example.com"`) into an IPv6 readable address format.



## 4.1 Networked Concurrency

In this chapter:
- define the fundamental building blocks of the TCP/IP Internet model, including its structure as a layered architecture.
- describe the key components and architectural structures of network applications
- examine how the socket API is used to implement network applications
- implement basic features of three protocols (HTTP, DNS, DHCP) that demonstrate key features of TCP and UDP socket programming

## 4.2 The TCP/IP Internet Model

The Internet is not a single computer network. Rather, the Internet is a network of interconnected networks that communicate using a common set of protocols. The messages originate with hosts - computers and similar connected devices - at the network edge. The messages are then passed (or forwarded) between routers, devices that are connected in a way that creates the structure of the network. The network core that serves as the basis of the Internet is the backbone, which refers to the connected networks that are controlled and maintained by a few telecommunications companies. 

### 4.2.1 Internet Model

Application - Transport - Internet - Link - Physical

The application layer opens a socket, the software endpoint of the connection. Sockets can be considered a form message passing IPC that is primarily used for network communication.

The transport layer breaks application layer messages-which may be large-into fixed-size data segments for delivery. The transport layer then provides a multiplexing service, allowing all processes on a single host to share a single network connection. At the receiving end, the transport layer performs demultiplexing, directing the data to the appropriate process. Multiplexing and demultiplexing are achieved through the use of a port number, which is an integer associated with a particular process. 

Transmission Control Protocol (TCP), provides flow control and reliable transport services.

User Datagram Protocol (UDP) is a connectionless protocol that does not maintain state between messages; each message that arrives at a server is treated as if it is from a new, unique client. 

The Internet layer provides the logical structure of the connections between hosts. The Internet layer provides point-to-point communication between the hosts and the routers, both within and across network boundaries.

The lowest layers of this model are the link and physical layers, which are typically designed and implemented together in hardware devices. 

### 4.2.2 Packet Encapsulation and Nomenclature

Transport layer -> Segment
Internet layer -> Packet
Link layer -> Frame

The term router is used to describe a device that operates at the Internet layer; a switch only operates as high as the link layer and does not provide the higher layers of the protocol stack.

### 4.3 Network Applications and Protocols

### 4.3.1 Naming and Addressing

A process must provide information about which host and process to contact. This information is normally provided to the client process by the user in the form of a uniform resource identifier (URI). URI adhere to a common structure:

```
URI = scheme:[//authority]path[?query][#fragment]
```

The scheme component defines the application-layer protocol that is being used; common examples of schemes would be using http or https for web pages, ftp for file transfers, or ssh to connect to a remote terminal on another host. 

The path component consists of a sequence of data fields, typically organized to create a hierarchy of resources, joined together by a delimiter. 

The bracket notation in the URI indicates that the other components are optional, though some application protocols may require them. If these components are present, they begin with a particular identifying character sequence. The query and fragment components begin with the '?' and '#' characters, respectively, and they can be used to customize the data requested. For instance, in a web page that is dynamically constructed by the server, the query might be used to provide user input. One example of this would be an e-commerce application that uses a URI such as `ttp://mystore.com/itemLookup?itemID=53` to direct the request to a single server-side file (`itemLookup`) that performs a database query for item 53, showing its picture, price and other information.

The fragment component is typically used to direct the client application to customize the view in some way. For instance, a web browser using the fragment field may jump to a section identified in the HTML source with an anchor tag or an ID attribute, rather than displaying the default top of the page. 

The authority component begins with a double slash ('//') and adheres to the following structure:

```
authority = [userinfo@]host[:port]
```

The `host` field is either a domain name or an IP address that is used to locate the host within the network. 

The `port` number is used to identify the process that should receive the application data from the packet. Many protocols have a default port number, such as using port 80 for a web browser communicating via HTTP. 

The `userinfo` field is used if a user identity needs to be associated with the request, as when using SSH to log into a user's account.

### 4.3.2 Protocol Specifications and RFCs

One key feature that is common is the use of :ackus-Naur Form (BNF), a formal method for defining information about a language or a set of messages. Reading BNF is a key skill for understanding the protocol specifications in RFCs. As an example, consider the following portion of the HTTP/1.1 specification in RFC 2616:

```
HTTP-message    = Request | Response
Request         = Request-line
                  *(( general-header
                   | request-header
                   | entity-header ) CRLF)
                  CRLF
                  [ message-body ]
Request-Line    = Method SP Request-URI SP HTTP-Version CRLF
```

The text to the left of an equal sign ('=') declare a unique type of enitity that serves as a basic language feature; in this case, these lines define the structure of the entities `HTTP-message`, `Request` and `Request-Line`. 

The vertical bar ('|') denotes a choice operation that can be satisfied by either entity; for instance, an HTTP-message can be either a `Request` or a `Response`. 

Consecutive lines indicate multiple entities that are required in a particular sequence; the line breaks in BNF do not have a specific meaning and only serve the purpose of making the definition more readable. 

The parantheses are used for grouping entities

The `Kleene star` character ('*') indicates that an entity may appear zero or more times, and the brackets indicate an optional entity that may appear once or may be omitted. 

The CRLF entity denotes the character sequence "carriage return-line feed" that consists of ASCII character 13 ('\r') followed by ASCII character 10 ('\n'), and the SP entity denotes a single space (ASCII code 32, ' ').

### 4.4.3 Server Socket Interface

Setting up a server socket involves a different sequence of steps from the client process. As before, `getaddrinfo()` provides an interface for configuring the socket address information, but the arguments will be structured differently. For starters, we are not using `getaddrinfo()` to perform a DNS query on a remote host; instead, we are setting up the socket based on the current host's existing IP address. In addition, if the server is part of a custom application, rather than a standard utility like a web server, the port number will not be identified as one of the well-known ports.

Once the socket address information has been configured, the process can then make a sequence of function calls to become a server. Typically (though not required), the first call is to `setsockopt()` to configure the socket with the `SO_REUSEADDR` option. This option avoids a common error during the next step, `bind()`. The `bind()` call links the port number with the current process. Sometimes when a port number is reused, a timing problem can cause the previous process (which is no longer running) to fail to release the port fully. Setting the `SO_REUSEADDR` option tells `bind()` to ignore this and forcefully replace the port association.

Connection-oriented TCP sockets require two additional function calls. The first, `listen()`, converts the socket to a connection-oriented server socket with a designated request queue. 

Figure illustrates how the timing of the client and server function relate. Both processes independetly set up their sockets. The server then executes the sequence of calling `bind()`, `listen()` and `accept()`. 

The call to `accept()` is blocking, so the server would then wait until a connection request arrives. 

When the request arrives, `accept()` would collect information about the client host and return.

### 4.4.4 Socket Communication

For TCP sockets, exchanging messages between the client and server can be done using the standard `read()` and `write()` operations, as with other forms of IPC. This works because sockets are treated like files, and the value returned from `socket()` behaves the same way as any other file descriptor. 

UDP sockets require the use of `recvfrom()` and `sendto()` for data exchange. These functions use `struct sockaddr` parameters to determine the sender's IP address when receiving and to specify the destination when sending. 

## 4.5 TCP Socket Programming: HTTP

Processes running at the application layer of the protocol stack are not fundamentally different from non-networked concurrent applications. The process has a virtual memory space, can exchange data through IPC channels, may interact with users through `STDIN` and `STDOUT`, and so on. The primary difference between such distributed application processes and non-networked processes are that the data is exchanged via an IPC channel based on a predefined communication protocol, and that channel has a significantly higher likelihood of intermittent communication failures. 

### 4.5.1 Hypertext Transfer Protocol (HTTP)

The `netcat` tool is a useful way to explore the details of HTTP without a web browser. 

To use `netcat`, you specify the hostname (example.com) and the port number (80) to access. After the command prompt, the first two lines are printed by `netcat` (in verbose mode with the `-v` flag) to indicate it has connected to the server. The next four lines (the `GET`, `Host`, `Connection`, and blank lines) were typed manually by the user to request the contents of `example.com`; there are several other domains that can be accessed from the same IP address. 

### 4.5.2 BNF Protocol Specification

The key features of the HTTP specification in 


### 4.5.4 Processing HTTP Headers

Although writing HTTP headers is straightforward, reading them at the other end can be a challenge if not handled properly. The difficulty arises from the fact that header sizes vary, so the receiver does no know how many bytes to request from the socket at a time. To address this challenge, both clients and servers typically impose a a maximum header size of 8 KB by convention. The initial read from the socket requests this much data. If a complete header is not found in this space, then the connection is terminated as invalid by clients; servers that receive such invalid headers return Status 413 to indicate `Entity Too Large`. A complete header must end with a blank line, creating the four-byte sequence "\r\n\r\n". 

Once the header and body have been split, processing the header involves repeatedly breaking it at the `CRLF` locations. 

### 4.5.5 Persistent State with Cookies

## 4.6 UDP Socket Programming: DNS

### 4.6.1 Resolving DNS Queries

### 4.6.4 Constructing DNS Queries with Sockets

```cpp
typedef struct {
  uint16_t xid;     /* Randomly chosen identifier */
  uint16_t flags;   /* Bit-mask to indicate request/response */
  uint16_t qdcount; /* Number of questions */
  uint16_t ancount; /* Number of answers */
  uint16_t nscount; /* Number of authority records */
  uint16_t arcount; /* Number of additional records */
} dns_header_t;

typedef struct {
  char *name;       /* Pointer to the domain name in memory */
  uint16_t dnstype; /* The QTYPE (1 = A) */
  uint16_t dnsclass;/* The QCLASS (1 = IN) */
}dns_question_t
```

Illustrates how to start creating a DNS query using the OpenDNS service. This same request could be sent to any DNS server, such as the DNS server operated by the reader's ISP. 

Open a socket (SOCK_DGRAM) to create a UDP socket. 

OpenDNS's DNS server IPv4 address is available at 208.67.222.222, which is the hexadecimal value 0xd043dede. 

DNS servers listen on port 53, so that value is also set. 

For the DNS header, we can randomly assign any value to the XID field, which has no inherent meaning to the server itself. 

### 4.6.5 Processing DNS Query Responses

To receive the response from the DNS server, start by allocating the contents of a 512-byte buffer in memory. The length of this buffer can be hard-coded in this way, as the DSN specification mandates a maximum of 512 bytes for all messages. The actual length of the received data is set when `recvfrom()` retrieves the response from the socket.

The use of `__attribute__((packed))` in this `struct` declaration is critical to tell the compiler not to re-order the fields of the `struct` within the program. When reading data from the network, the bytes will occur in a particular order. When we use a `struct` to impose a logical meaning on those bytes in a program, we would expect the interpretation to be like this:

However, compilers routinely re-order the fields in a `struct` to preserver word alignment, trying to group the bytes into chunks of 32 bits as much as possible. In this case, many compilers would swap the `ttl` and `length` fields, which would impose the wrong structure on the sequence of bytes received from the network:

## 4.7 Application-Layer Broadcasting: DHCP

DNS provides a mechanism that clients can use to determine the IP address for a server based on a human-readable domain name. In addition, server processes for common protocols listen for incoming requests on well-known ports. 































