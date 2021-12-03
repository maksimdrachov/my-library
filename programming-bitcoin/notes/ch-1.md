## Chapter 1: Finite Fields

This chapter is going to get you off to a manageable start. It may seem strange, but we'll start with the basic math that you need to understand elliptic curve cryptography. Elliptic curve cryptography, in turn, gives us the signing and verification algorithms. These are at the heart of how transactions work, and transactions are the atomic unit of value transfer in Bitcoin. By learning about finite fields and elliptic curve first, you'll get a firm grasp of concepts that you'll need to progress logically. 

This chapter is required if you want to understand elliptic curve cryptography. Elliptic curve cryptography is required for understanding signing and verification, which is at the heart of Bitcoin itself. The fundamentals here will not only make understanding Bitcoin a lot easier, but also make understanding Schnorr signatures, confidential transactions, and other leading-edge Bitcoin technologies easier. 

### Finite Field Definition

Mathematically, a _finite field_ is defined as a finite set of numbers and two operations + (addition) and * (multiplication) that satisfy the following:

1. If a and b are in the set, a+b and a*b are in the set. We call this property _closed_.
2. 0 exists and has the property a+0=a. We call this the _additive identity_.
3. 1 exists and has the property a$1=a. We call this the _multiplicative identity_.
4. If a is in the set, -a is in the set, which is defined as the value that makes a+(-a)=0. This is what we call the _additive inverse_.
5. If a is in the set and is not 0, a^(-1) is in the set, which is defined as the value that makes a*a^(-1)=1. This is what we call the _multiplicative inverse_.

Let's unpack each of these criteria. 

We have a set of numbers that's infinite. Because the set is finite, we can designate a number p, which is how big the set is. This is what we call the _order_ of the set.

#1 says we are closed under addition and multiplication. This means that we have to define addition and multiplication in a way that ensures the results stay in the set. For example, a set containing {0,1,2} is _not_ closed under addition, since 1+2=3 and 3 is not in the set. Of course we can define addition a little differently to make this work, but using "normal" addition, this set is not closed. On the other hand, the set {-1,0,1} is closed under normal multiplication. 

The other option we have in mathematics is to define multiplication in a particular way to make these sets closed. We'll get to how, but the key concept is that we can define addition and subtraction differently than the addition and subtraction you are familiar with.

#2 and #3 mean that we have the additive and multiplicative identities. That means 0 and 1 are in the set. 

#4 means that we have the additive inverse. That is, if a is in the set, -a is in the set. Using the additive inverse, we can define subtraction. 

#5 means that multiplication has the same property. If a is in the set, a^(-1) is in the set. That is a*a^(-1)=1. Using the multiplicative inverse, we can define division. This will be the trickiest to define in a finite field. 

### Defining Finite Sets

If the order (or size) of the set is p, we can call the elements of the set, 0, 1,..., p-1. These numbers are what we call the _elements_ of the set, not necessarily the traditional numbers 0,1,2,3, etc. They behave in many ways like traditional numbers, but have some differences in how we add, subtract, multiply, and so forth. 

In math notation the finite field set looks like this:

```
F_p = {0, 1, 2,..., p-1}
```

What's in the finite field set are the elements. F_p is a specific finite field called "field of p" or "field of 29" or whatever the size of it is (again, the size is what mathematicians call order). 

A finite field of order 11 looks like this:

```
F_11 = {0,1,2,3,4,5,6,7,8,9,10}
```

Notice the order of the field is always 1 more than the largest element. You might also have notices that the field has a prime order every time. For a variety of reasons that will become clear later, it turns out that fields _must_ have an order that is a power of a prime, and that the finite fields whose order is prime are the ones we're interested in. 

### Constructing a Finite Field in Python

We want to represent each finite field element, so in Python, we'll be creating a class that represents a single finite field element. Naturally we'll name the class `FieldElement`.

The class represents an element in a field F_prime. 

#### Exercise 1 

Write the correspondin `__ne__`, which checks if two `FieldElement` objects are not equal to each other.

```python
def __ne__(self, other):
    if other is None:
        return True
    return not(self.num == other.num and self.prime == other.prime)
```

### Module Arithmetic

One of the tools we can use to make a finite field closed under addition, subtraction, multiplication, and division is something called _module arithmetic_.

### Finite Field Addition and Subtraction

We denote finite field addition with +_f to avoid confusion with normal integer addition, +.

If we utilize module arithmetic, we can guarantee this to be the case. We can define a +_f b this way:

```
a +_f b = (a+b)%19
```

More generally, we define field additions this way:

```
a +_f b = (a+b)%p
```

We can also define the additive inverse this way. 

```
-_f a = (-a)%p
```

#### Exercise 2

Solve these problems in F_57:
```
# 44 + 33
(44+33)%57

# 9 - 29
(9-29)%57

# 17 + 42 + 49
(17+42+49)%57

# 52-30-38
(52-30-38)%57
```

### Coding Addition and Subtraction in Python

In the field FieldElement we can now define `__add__` and `__sub__` methods. 

In Python we can define what addition (or the + operator) means for our class with the `__add__` method.

```
def __add__(self, other):
    if self.prime != other.prime:
        raise TypeError('Cannot add two numbers in different fields')
    num  = (self.num + other.num) % self.prime
    return self.__class__(num, self.prime)
```

3: We have to return an instance of the class, which we can conveniently access with `self.__class__`. We pass the two initializing arguments, num and self.prime, for the `__init__` method we saw earlier.

Note that we could use `FieldElement` instead of `self.__class__`, but this would not make the method easily inheritable. We will be subclassing `FieldElement` later, so making the method inheritable is important here.

#### Exercise 3

Write the corresponding `__sub__` method that defines the subtraction of two `FieldElement` objects. 

```python
def __sub__(self, other):
    if self.prime != other.prime:
        raise TypeError('Cannot subtract two numbers in different fields')
    num = (self.num - other.num) % self.prime
    return self.__class__(num, self.prime)
```

### Finite Field Multiplication and Exponentiation

Just as we defined a new addition for finite fields that was closed, we can also define a new multiplication for finite fields that's closed. By multiplying the same number many times, we can also define exponentiation. In this section we'll go through exactly how to define this using module arithmetic.

#### Exercise 5

For k = 1,3,7,13,18, what is this set in F_19?

They are all the same.

_Why Prime Fields are Useful_

The answer to exercise 5 is why we choose to use finite fields with a _prime_ number of elements. No matter what k you choose, as long as it's greater than 0, multiplying the entire set by k will result in the same set as you started with. 

Intuitively, the fact that we have a prime order results in every element of a finite field being equivalent. If the order of the set was a composite number, multiplying the set by one of the divisors would result in a smaller set. 

### Coding Multiplication in Python

### Coding Exponentiation in Python

We need to define the exponentiation for `FieldElement`, which in Python can be defined with the `__pow__` method, overriding the ** operator. The difference here is that the exponent is _not_ a `FieldElement`, so it has to be treated a bit differently. 

```
class FieldElement:

    def __pow__(self, exponent):
        num = (self.num ** exponent) % self.prime
        return self.__class__(num, self.prime)
```

This is a perfectly fine way to do it, but pow(self.num, exponent, self.prime) is more efficient. 

### Finite Field Division

The intuition that helps us with addition, subtraction, multiplication, and perhaps even exponentiation unfortunately doesn't help us quite as much with division. 

In normal math, division is the inverse of multiplication.

Dividing any two number where the denominator is not 0 will result in another finite field element.

Fermat's little theorem:

```
n^(p-1) % p = 1
```

This is a relatively expensive calculation as exponentiating grows very fast. Division is the most expensive operation for that reason. To lessen the expense, we can use the pow function in Python, which does exponentiation. In Python, pow(7,17) does the same thing as 7**17. The pow function, however, has an optional third argument that makes our calculation more efficient. Specifically, pow will modulo by the third argument. Thus, pow(7,17,19) will give the same result as `7**17%19` but do so faster because the module operator is done after each round of multiplication.

#### Exercise 8

```
# 3/24
3*pow(24,29,32)%31

# 17**-3
pow(17, 31-4, 31)

# 4**-4*11
pow(4, 34-5, 31)*11%31
```

#### Exercise 9

Write the corresponding `__truediv__` method that defines the division of two field elements. 

### Redefining Exponentiation

### Conclusion 

In this chapter we learned about finite fields and how to implement them in Python. We'll be using finite fields in Chapter 3 for elliptic curve cryptography. We turn next to the other mathematical component that we need for elliptic curve cryptography: elliptic curves. 

