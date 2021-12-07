## Chapter 5: Transactions

### Transaction Components

At a high level, a transaction really only has four components. They are:

1. Version 
2. Inputs
3. Outputs
4. Locktime

The version indicates what additional features the transaction uses, inputs define what bitcoins are being spent, outputs define where the bitcoins are going, and locktime defines when this transaction starts being valid. 

### Version

#### Exercise 4

Write the version parsing part of the `parse` method that we've defined. To do this properly, you'll have to convert 4 bytes into a little-endian integer. 

### Inputs

Each input points to an output of a previous transaction.

Bitcoin's inputs are spending outputs of a previous transaction. That is, you need to have received bitcoins first to spend something. This makes intuitive sense. You can not spend money unless you've received money first. The inputs refer to bitcoins that belong to you. Each input needs two things:

- A reference to bitcoins you've received previously
- Proof that these are yours to spend

The second part uses ECDSA (chapter 3). You don't want people to be able to forge this, so most inputs contain signatures that only the owner(s) of the private keys can produce.

The input field can contain more than one input.

We can see that the byte is actually 01, which means that this transaction has one input. It may be tempting here to assume that it's always a single byte, but it's not. A single byte has 8 bits, so anything over 255 inputs will not be expressible in a single byte. 

This is where _varints_ come in. Varint is shorthand for _variable integer_, which is a way to encode an integer into byte that range from  0 to 2^64-1. 

Each input contains four fields. The first two fields point to the previous transaction output and the last two fields define how the previous transaction output can be spent. The fields are as follows:

- Previous transaction ID
- Previous transaction index
- ScriptSig
- Sequence

Each input has a reference to previous transactions output. The previous transaction ID is the hash256 of the previous transaction's contents. This uniquely defines the previous transaction, as the probability of a hash collision is impossibly low. 

As we'll see, each transaction has to have at least one output, but may have many. Thus, we need to define exactly which output _within a transaction_ we're spending, which is captured in the previous transaction index. 

Note that the previous transaction is 32 bytes and that the previous transaction index is 4 bytes. Both are little-endian.

The ScriptSig has to do with Bitcoin's smart contract language, Script, discussed more thoroughly in Chapter 6. For now, think of the the ScriptSig as opening a locked box - something that can only be done by the owner of the transaction output. The ScriptSig field is a variable-lenght field, not a fixed-length field like most of what we've seen so far. A variable-length field requires us to define exactly how long the field will be, which is why the field is preceded by a varint telling us how long it is. 

