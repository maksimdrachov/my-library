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









































## 4.4 The Socket Interface

### 4.4.1 Networking Data Structures

Once the socket is created, the client and server processes use different functions to establish the network link between their sockets. These functions rely on a common `struct sockaddr` data structure. The basic form of this structure contains two fields:

```cpp
/* defined in sys/socket.h */
struct sockaddr {  /* generic socket address structure */
  sa_family_t sa_family;
  char sa_data[14];
};
```

The `sockaddr` and `sockaddr_in` structures are identical in size, allowing for straightforward casting between the two. Both begin with a `sa_family_t` field to indicate the domain. The `sockaddr_in` breaks the rest of the bytes into three fields. The `sin_port` is a 16-bit field to designate the port number for the socket and the `sin_addr` contains the 32-bit IPv4 address.

The internal representations of `in_addr` and `in6_addr` are different from the standard notation used for IP addresses. For instance, readers may be familiar with the `dotted decimal` notation of IPv4 addresses, as illustrated by the loopback address 127.0.0.1 that refers to the local host machine. This format is used for human readability, but the actual IP address is stored as a 32-bit valuem with each byte corresponding to one the dotted fields.

### 4.4.2 Client Socket Interface

IP addresses should not be hard-coded, as they can change. 

Instead, `getaddrinfo()` provides an interface to look up an IP address by the standard text format used in URIs. This string is passed as the first argument, `nodename`.

The `servname` parameter indicates a desired service, such as `"http"`. 

The `hints` parameter can be used to limit the list of results, such as restricting the domain to `AF_INET` (IPv4) or `AF_INET6` (IPv6), or limiting the type to `SOCK_STREAM` or `SOCK_DGRAM`.

The final parameter, `res`, is a call-by-reference parameter that will be set to point to a linked list of address structures.

### Server Socket Interface

