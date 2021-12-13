## Chapter 3: How the LN works

The LN is a peer-to-peer network of payment channels implemented as smart contracts on the Bitcoin blockchain as well as a communication protocol that defines how participants set up and execute these smart contracts.

A payment channel is a financial relationship between two nodes on the Lightning Network, called the channel partners. The financial relationship allocates a balance of funds, between the two channel partners.

The payment channel is managed by a cryptographic protocol, meaning a predefined process based on cryptography is used by the channel partners to redistribute the balance of the channel in favor of one or the other channel partner. The cryptographic protocol ensures that one channel partner cannot cheat the other, so that the partners do not need to trust each other.

The cryptographic protocol is established by the funding of a 2-of-2 multisignature address that requires the two channel partners to cooperate and prevents either channel partner from spending the funds unilaterally.

To summarize: a payment channel is a financial relationship between nodes, allocating funds from a multisignature address through a strictly defined cryptographic protocol.

If you have an unpublished transaction from a 2-of-2 multisignature address that pays you part of the balance, then a signature from the other party ensures that you can independently publish this transaction anytime by adding your own signature.

The ability to hold a partially signed transaction, offline and unpublished, with the option to publish and own that balance at any time, is the basis of the Lightning Network.

A channel is only limited by three things:

1. First, the time it takes for the internet to transfer the few hundred bytes of data that the protocol requires to move funds from one end of the channel to the other.
2. Second, the capacity of the channel, meaning the amount of bitcoin that is committed to the channel when it is opened. 
3. Third, the maximum size limit of a Bitcoin transaction also limits the number of incomplete (in progress) routed payments that can be carried simultanously over a channel

Payment channels have a few very interesting and useful properties:

- The cryptographic protocol is constructed such that there is little to no trust needed between you and your channel partner. If your partner becomes unresponsive or tries to cheat you, you can ask the Bitcoin system to act as a "court", resolving the smart contract you and your partner have previously agreed upon.

[The Bitcoin Lightning Network paper](https://lightning.network/lightning-network-paper.pdf)

### Multisignature Address

Payment channels are built on top of 2-of-2 multisignature addresses.

The amount deposited in the multisignature address is called the channel capacity and sets the maximum amount that can be sent across the payment channel. 

There are some disadvantages to moving funds into the Lightning Network (such as the need to keep them in a "hot" wallet), but the idea of "locking funds" in Lightning is misleading.

What if you channel partnet refuses to sign a transaction to releas the funds?

To prevent this, Alice will need to create an additional transaction that spends from the multisignature address, refunding her mBTC. Alice then has Bob sign the refund transaction _before_ broadcasting her funding transaction to the Bitcoin network. This way, Alica can get a refund even if Bob disappears or fails to cooperate.

The "refund" transaction that protects Alice is the first of a class of transaction called _commitment transactions_, which we will examine in more detail next.

Commitment transactions are created each time the channel balance changes. In other words, each time a payment is sent between Alice and Bob, new commitment transactions are created and signatures are exchanged. Each new commitment transaction encodes the latest balance between Alice and Bob.

Now that we understand _why_ a penalty mechanism is needed and how it will prevent cheating, let's see _how_ it works in detail.

Usually, the commitment transaction has at least two outputs, paying each channel partner. We can change this to add a timelock delay and a revocation secret to one of the payments. The timelock prevents the owner of the output from spending it immediately once the commitment transaction is included in a block. The revocation secret allows either party to immediately spend that payment, bypassing the timelock.

So, in our example, Bob holds a commitment transaction that pays Alcie _immediately_, but his own payment is delayed and revocable. Alice also holds a commitment transaction, but hers is the opposite: it pays Bob immediately but her own payment is delayed and revocable.

The timelock is set to a number of blocks up to 2,016 (approximately two weeks). If either channel partner publishes a commitment transaction without cooperating with the other partner, they will have to wait for that number of blocks (e.g. two weeks) to claim their balance. 

The timelock is adjustable and can be negotiated between channel partners. Usually, it is longer for larger capacity channels, and shorter for smaller channels, to align the incentives with the value of the funds.

For every new update of the channel balance, new commitment transactions and new revocation secrets have to be created and saved. As long as a channel remains open, all revocation secrets _ever created_ for the channel need to be kept because they might be needed in the future. Fortunately, the secrets are rather small and it is only the channel partners who need to keep them, not the entire network. Furthermore, due to a smart derivation mechanism used to derive revocation secrets, we only need to store the most recent secret, because previous secrets can be derived from it (see revocation_secret_derivation).

Nevertheless, managing and storing the revocation secrets is one of the more elaborate parts of Lightning nodes that require node operators to maintain backups.

Technologies such as watchtower services or changing the channel construction protocol to the eltoo protocol might be future strategies to mitigate these issues and reduce the need for revocation secrets, penalty transactions, and channel backups.

### Announcing the channel

Announcing channels publicly allows other nodes to use them for payment routing, thereby also generating routing fees for the channel partners. 

So, how do you catch a cheat or a protocol breach in your day-to-day activities? You do so by running software that monitors the public Bitcoin blockchain for on-chain transactions that correspond to any commitment transaction for any of your channels. This software is one of three types:

- A properly maintained Lightning node, running 24/7
- A single-purpose watchtower ndoe that you run to watch your channels
- A third-party watchtower node that you pay to watch your channels

As long as you run your Lightning node once before the timeout period is reached, it will catch all cheating attempts. It is not advisable to take this kind of risk, it is important to keep a well-maintained node running continuously (see continuous_operation, bad link)

### Invoices

Most payments on the Lightning Network start with an invoice, generated by the recipient of the payment. 

An invoice is a simple payment instruction containing information such as unique payment identifier (called a payment hash), a recipient, an amount, and an option text description.

The most important part of the invoice is the payment hash, which allows the payment to travel across multiple channels in an _atomic_ way. 

Invoices are not communicated over the Lightning Network. Instead, they are communicated "out of band", using any other communication mechanism. Can present a Lightning invoice to Alice as a QR code, via email, or through any other message channel.

Invoices are usually encoded as a long _bech32_-encoded string or as a QR code. The sender uses the signature to extract the public key (also known as the node id) of the recipient so that the sender knows where to send the payment. 

### Payment hash and preimage

The most important part of the invoice is the payment hash. When constructing the invoice, Bob will make a payment hash as follows:

1. Bob chooses a random number r. This random number is called the preimage or payment secret.
2. Bob uses SHA-256 to calculate the hash H of r called the payment hash.

### Pathfinding and routing

### Comparison with Bitcoin

#### Addresses versus invoices, transactions versus payments

In Lightning, however, each invoice can only be used once for a specific payment amount. You cannot pay more or less, you cannot use an invoice again, and the invoice has an expiry time built in. In Lightning, a recipient has to generate a new invoice for each payment, specifying the payment amount in advance. 

#### Selecting outputs versus finding a path

#### Change outputs on bitcoin versus no change on lightning

#### Mining fees versus routing fees

#### Varying fees depending on traffic versus announced fees

#### Public bitcoin transactions versus private lightning payments

#### Waiting for confirmations versus instant settlement

#### Sending arbitrary amounts versus capacity restrictions

#### Incentives for large value payment versus small value payment

#### Using the Blockchain as a ledger versus as a court system

#### Offline versus online, asynchronous versus synchronous

#### Satoshis versus milisatoshis

