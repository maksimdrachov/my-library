## Chapter 3: Elliptic Curve Cryptography

In this chapter, we're going to combine the two concepts to learn elliptic curve cryptography. Specifically, we're going to build the primitives needed to sign and verify messages, which is at the heart of what bitcoin does.

### Elliptic Curves over Reals

We discussed in Chapter 2 what an elliptic curve looks like visually because we were plotting the curve over _real_ numbers. Specifically, it's not just integers or even rational numbers, but all real numbers. 

This worked because real numbers are also a field. Unlike a _finite_ field, there are an _infinite_ number of real numbers, but otherwise the same properties hold:

It turns out we can use the point addition equations over any field, including the finite fields we learned about in Chapter 1. The only difference is that we have to use the addition/subtraction/multiplication/division as defined in Chapter 1, not the "normal" versions that the real numbers use.

### Elliptic Curves over Finite Fields

So what does an elliptic curve over a finite field look like? Let's look at the equation y^2 = x^3 + 7 over F_103. We can verify that the point (17,64) is on the curve by calculating both sides of the equation:

We've verified that the point is on the curve using finite field math.

Because we're evaluating the equation over a finite field, the plot of the equation looks vastly different. 

#### Exercise 1

### Coding Elliptic Curves over Finite Fields

Because we defined an elliptic curve point and defined the +,-,* and / operators for finite fields, we can combine the two classes to create elliptic curve points over a finite field. 

### Point Addition over Finite Fields

### Coding Point Addition over Finite Fields

Because we coded `FieldElement` in such a way as to define `__add__`, `__sub__`, `__mul__`, `__truediv__`, `__pow__`, `__eq__` and `__ne__`, we can simply initialize `Point` with `FieldElement` objects and point addition will work.

#### Exercise 2

#### Exercise 3

### Scalar Multiplication for Elliptic Curves

One property of scalar multiplication is that it's really hard to predict without calculating.

Each point is labeled by how many times we've added the point. You can see that this is a complete scattershot. This is because the point addition is nonlinear and not easy to calculate. Performing scalar multiplication is straightforward, but doing the opposite, point division, is not.

This is called the discrete log problem and is the basis of elliptic curve cryptography.

Another property of scalar multiplication is that at a certain multiple, we get to the point at infinity (remember, the point at infinity is the additive identity or 0). If we imagine a point G and scalar multiply until we get to the point at infinity, we end up with a set. 

It turns out that this set is called a group, and because n is finite, we have a finite group (or more specifically, a finite cyclic group). Groups are interesting mathematically because they behave well with respect to addition.

When we combine the fact that scalar multiplication is easy to do in one direction but hard in the other and the mathematical properties of a group, we have exactly what we need for elliptic curve cryptography.

#### Why is this called the discrete log problem?

You may be wondering why the problem of reversing scalar multiplication is referred to as the discrete log problem.

The log equation on the left has no analytically calculable algorithm. That is, there is no known formula that you can plug in to get the answer generally. 

### Scalar Multiplication Redux

Scalar multiplication looks really random, and that's what gives this equation asymmetry. An assymetric problem is one that's easy to calculate in one direction, but hard to reverse. For example, it's easy enough to calculate 12*(47,71). But if we were presented with this:

```
s*(47,71)=(194,172)
```

would we be able to solve for s? We can look up the results shown earlier, but that's because we have a small group. 

### Mathematical Groups

What we actually want to generate for the purposes of public key cryptography are finite cyclic groups, and it turns out that if we take a generator point from an elliptic curve over a finite field, we can generate a finite cyclic group. 

Unlike fields, groups have only a single operation. In our case, point addition is the operation. Groups alos have a few other properties, like closure, invertiblity, commutativity, and associativity. Lastly, we need the identity.

### Identity

The identity is defined as the point at infinity, which is guaranteed to be in the group since we generate the group when we get to the point at infinity. So:

```
0 + A = A
```

We call 0 the point at infinity because visually, it's the point that exists to help the math work out.

### Closure

This is perhaps the easiest property to prove since we generated the group in the first place by adding G over and over. Thus, if we have two different elements that look like this:

```
aG + bG = (a+b)G 
```

How do we know this element is in the group? If `a+b < n` (where n is the order of the group), the we know it's in the group by definition. If a+b >= n, the we know a < n and b < n, so a+b < 2n, so a+b-n<n:

```
(a+b-n)G = aG + bG - nG = aG + bG - 0 = aG + bG
```

More generally, (a+b)G = ((a+b)%n)G, where n is the order of the group.

So we know that this element is in the group, providing closure.

### Invertibility

Mathematically, we know that if aG is in the group, (n-a)G is also in the group. You can add them together to get aG + (n-a)G = (a+n-a)G = nG = 0.

### Commutativity

This means that aG + bG = bG + aG, which proves commutativity.

### Associativity

We know from point addition that A + (B +C) = (A + B) + C

#### Exercise 5

### Coding Scalar Multiplication

There's a cool technique called _binary expansion_ that allows us to perform multiplication in log2(n) loops, which dramatically reduces the calculation time for large numbers. For example, 1 trillion is 40 bits in binary, so we only have to loop 40 times for a numbers that's generally considered very large.

### Defining the Curve for Bitcoin

While we've been using relatively small primes for the sake of samples, we are not restricted to such small numbers. Small primes mean that we can use a computer to search through the entire group. If the group has a size of 301, the computer can easily do 301 computations to reverse scalar multiplication or break discrete log. 

But what if we made the prime larger? It turns out that we can choose much larger primes than we've been using. The security of elliptic curve cryptography depens on computers not being able to go through an appraciable fraction of the group. 

An elliptic curve for public key cryptography is defined with the following parameters:

- We specify the a and b of the curve y^2 = x^" + ax + b
- We specify the prime of the finite field, p.
- We specify the x and y coordinates of the generator point G.
- We specify the order of the group generated by G, n.

These numbers are known publicly and together form the cryptographic curve. There are many cryptographic curves and they have different security/convenience trade-offs, but the one we're most interested in is the one Bitcoin uses: secp256k1. The parameters for secp256k1 are these:

- a = 0n b =7, making the equation y^2 = x^3 + 7
- p = 2^256 - 2^32 - 977
- G_x = 0x795e....
- G_y = 0x483a....
- n = 0xff...

There are a few things to notice about this curve. First, the equation is relatively simple. Many curves have a and b values that are much bigger. 

Second, p is extremely close to 2^256. This means that most numbers under 2^256 are in the prime field, and thus any point on the curve has x and y coordinates that are expressible in 256 bits each. n is also very close to 2^256. This means any scalar multiple can also be expressed in 256 bits. 

### Working with secp256k1

### Public Key Cryptography

At last, we have the tools that we need to do public key cryptography operations. The key operation that we need is P = eG, which is an asymmetric equation. We can easily compute P when we know e and G, but we cannot easily compute e when we know P and G. This is the dircrete log problem.

Generally, we call e the private key and P the public key. Note here that the private key is a single 256-bit number and the public key is a coordinate (x,y), where x and y are each 256-bit numbers.

### Signing and Verification

### Inscribing the Target

The inscribing of the target depends on the _signature algorithm_, and in our case that algorithm is called the Elliptic Curve Digital Signature Algorithm, or ECDSA for short. 

The secret in our case is e satisfying the following:

eG = P

where P is the public key, and e is the private key.

The target that we're going to aim at is a random 256-bit number, k. We then do this:

kG = R

R is now the target that we're aiming for. In fact, we're only going to care about the x coordinate of R, which we'll call r. You may have guessed already that r here stands for random.

We claim at this point that the following equation is equivalent to the discrete log problem:

uG + vP = kG

where k was chosen randomly, u,v != 0 can be chosen by the signer, and G and P are known. This is due to the fact that:

### Verification in Depth

