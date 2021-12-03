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