## Distributed Systems and IoT Architecture

- Network interfaces
- The Internet Protocols
- Transport Layer Security
- Application protocols

### Network interfaces

Many microcontrollers integrate the **Media Access Control (MAC)** portion of an Ethernet interface, so connecting a **physical layer transceiver (PHY)** would enable LAN access. 

### Ethernet

### Wi-Fi

Among all the possibilities in the wireless universe, 802.11 Wi-Fi is chosen for its high-speed, low-latency channel, and for the widest possible compatibility in a topology including personal computers and mobile devices. However the power requirement of a wi-fi transceiver can sometimes be difficult to afford for low-power devices. 

### Low-Rate Wireless Personal Area Networks (LR-WPAN)

Sensors mesh networks make large use of wireless technology to establish communication in a local geographical area. The 802.15.4 standard regulates the access to 2.4 GHz and sub-GHz frequencies to provide limited-range local area networks with a typical maximum bit rate of 250 Kbps, which can be accessed using low-cost, low-power transceivers. The media access is not based on an infrastructure, and supports contention resulution and collision detection at the MAC level, using a beaconing system. Each node can be addressed using two bytes, and the special address 0xFFFF is reserved for broadcast traffic, to reach all the nodes in visibility. The maximum payload size for 802.15.4 frames is fixed to 127 bytes, and thus is not possible to encapsulate full-size IP packets routed from an Ethernet of a wireless LAN link. Network protocol implementations that are capable of communicating through 802.15.4 interfaces either are application-specific, don't support Internet Protocol networking, or offer fragmentation and compression mechanisms to transmit and receive each packet across multiple wireless frames. 

While not specifically designed for IoT, and not directly compatible with classic IP infrastructures, there are multiple choices available to build networks on top of 802.15.4. In fact, while the standard specifies the MAC protocol for exchanging frames among nodes that are in visibility, multiple link-layer technologies, standard and non-standard, have been developed to define networks on top of 802.15.4.

### LR-WPAN industrial link-layer extensions

Thanks to the flexibility of the transceivers, and the capability of transmitting and receiving 802.15.4 raw frames, it is relatively easy to implement networking protocols for LR-WPANs.

In the pre-IoT era, the process automation industry had been the first to adpot the 802.15.4 technology, and for a long time had been searching for a standard protocol stack to enable 

### 6LoWPAN

### Selecting the appropriate network interfaces

When selecting a communication technology, there might be several aspects to take into account:

- Range of communication
- Bit rate required for data transfer
- Total cost of ownership
- Media-specific limitations, such as latency introduced by the transceiver
- Impact of the RF interference on the hardware deisgn requirements
- Maximum transfer unit
- Power consumption, energy footprint
- Protocols/standards supported for compatibility with third-party systems
- Compliance to internet protocols for integration in IoT systems
- Topology flexibility, dynamic routing, mesh network feasibility
- Security model
- Resources required to implement drivers and protocols for a specific technology
- Use of open standards to avoid lock-in for long-lived projects

