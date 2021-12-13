## Chapter 2: Getting started

### Lightning Nodes

The Lightning Network is accessed via software applications that can speak the LN protocol. A Lightning Network node (LN node) is a software application that has three important characteristics:
- Lightning nodes are wallets, so they can send and receive payments over the lightning network as well as on the bitcoin network.
- Nodes must communicate on a peer-to-peer basis with other Lightning nodes creating the network.
- Lightning nodes also need access to the Bitcoin blockchain (or other blockchains for other cryptocurrencies) to secure the funds used for payments.

Users have the highest degree of control by running their own Bitcoin node and Lightning node. However, Lightning nodes can also use a lightweight Bitcoin client, commonly referred to as a simplified payment verification (SVP), to interact with the Bitcoin blockchain.

### Lightning Wallets

The most common components of Lightning wallet software include:

- A keystore that holds secrets, such as private keys
- An LN node that communicates on the peer-to-peer network, as described previously
- A Bitcoin node that stores blockchain data and communicates with other Bitcoin nodes
- A database "map" of nodes and channels that are announced on the Lightning Network
- A channel manager that can open and close LN channels
- A close-up system that can find a path of connected channels from payment source to payment destination

Any wallet that outsources management of keys is called _custodial_. 

A _noncustodial_ or _self-custody_ wallet, by comparison, is one where the keystore is part of the wallet, and keys are controlled directly by the user. 

The most important questions to ask about a specific wallet are:

1. Does this Lightning walled have a full Lightning node or does it use a third-party Lightning node?
2. Full Bitcoin node?
3. self-custody or custodian?

Lightning wallets can be installed on a variety of devices, including laptops, servers and mobile devices. To run a full Lightning node, you will need to use a server or desktop computer, because mobile devices and laptops are usually not powerful enough in terms of capacity, processing,...

The category third-party Lightning nodes can again be subdivided:

1. Lightweight
2. None

