## Chapter 1: Introduction

### What is Bitcoin?

Bitcoin consists of:

- A decentralized peer-to-peer network (the Bitcoin protocol)
- A public transaaction ledger (the blockchain)
- A set of rules for independent transaction validation and currency issuance (consensus rules)
- A mechanism for reaching global decentralized consensus on the valid blockchain (PoW algorithm)

Hardware wallets are devices that operate a secure self-contained Bitcoin wallet on special-purpose hardware. They usually connect to a desktop or mobile device via USB cable or near-field-communication (NFC), and are operated with a web browser or accompanying software. By handling all Bitcoin-related operations on the specialized hardware, these wallets are considered very secure and suitable for storing large amounts of bitcoin.

**Lightweight client**

A lightweight client, also known as a simplified-payment-verificiation (SPV) client, connects to Bitcoin full nodes for access to the Bitcoin transaction information, but stores the user wallet locally and independently creates, validates and transmits transactions. Lightweight clients interact directly with the Bitcoin network, without an intermediary.

For the purposes of this book, we will be demonstrating the use of a variety of downloadable Bitcoin clients, from the reference implementation (Bitcoin Core) to mobile and web wallets. Some of the examples will require the use of Bitcoin Core, which, in addition to being a full client, also exposes APIs to the wallet, network and transaction services. If you are planning to explore the programmatic interfaces into the Bitcoin system, you will need to run Bitcoin Core, or one of the alternative clients.


