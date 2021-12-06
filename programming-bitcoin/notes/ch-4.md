## Chapter 4: Serialization

We want to communicate or store a `S256Point` or a `Signature` or a `PrivateKey`.

### Uncompressed SEC Format

We'll start with the `S256Point` class, which is the public key class. Recall that the public key in elliptic curve cryptography is really a coordinate in the form of (x,y).

There is already a standard for serializing ECDSA public keys, called _Standards for Efficient Cryptography_ (SEC) - and as the word "Efficient" in the name suggests, it has minimal overhead. 

#### Exercise 1

#### Compressed SEC Format

#### Exercise 2

### DER Signatures

Another class that we need to learn to serialize is Signature. Much like the SEC format, it needs to encode two different numbers, r and s. Unfortunately, unlike S256Point, Signature cannot be compressed as s cannot be derived solely from r.

The standard for serializing signatures is called _Distinguished Encoding Rules_ (DER) format. 

#### Exercise 3

### Base58

In the early days of Bitcoin, bitcoins were assigned to public keys specified in SEC format (uncompressed) and then were redeemed using DER signatures. For reasons we'll get to in Chapter 6, using this particular very simple script turned out to be both wasteful for storing unspent transaction outputs (UTXOs) and a little less secure than the scripts in more prominent use now. 

### Transmitting Your Public Key

There are three mayor considerations. The first is that the public key be readable (easy to hand-write and not too difficult to mistake say, over the phone). The second is that it's short. The third is that it's secure (not easy to make mistakes).

Why Base58 is on the way out

Bech32 uses a 32-character alphabet that's just numbers and lowercase letters, except 1, b, i and o. Thus far it's only used for Segwit.

#### Exercise 4

### Address Format

The 264 bits from compressed SEC format are still a bit too long, not to mention a bit less secure (see Chapter 6). To both shorten the address and increase security, we can use the ripemd160 hash.

#### Exercise 5

Find the addresses corresponding to the public keys whose private key secrets are.

### WIF Format

The private key in our case is a 256-bit number. Sometimes you want to transmit your private key from one wallet to another.

For this purpose, you can use Wallet Import Format (WIF). WIF is a serialization of the private key that's meant to be human-readable. WIF uses the same Base58 encoding that addresses use.

#### Exercise 6

### Big- and Little-Endian Redux

It will be very useful to know how big- and little-endian are done in Python, as the next few chapter will be parsing and serializing numbers to and from big-/little-endian quite a bit. 

Recall that the SEC format uses big-endian encoding, as do addresses and WIF. From Chapter 5 onward, we will use little-endian encoding a lot more. For this reason, we turn to the next two exercises. The last exercise is to create a testnet address yourself. 

#### Exercise 7

