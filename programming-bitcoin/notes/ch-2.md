## Chapter 2: Elliptic Curves

### Definition

Elliptic curves are like many equations you've seen since pre-algebra. They have y on one side and x on the other, in some form, elliptic curves have a form like this:

```
y^2 = x^3 + a*x + b
```

The only real difference between the elliptic curve and the cubic curve is the y^2 term on the left side. This has the ffect of making the graph symmetric over the x-axis. 

Specifically, the elliptic curve used in Bitcoin is called secp256k1 and it uses this particular equation:

```
y^2 = x^3 + 7
```

### Coding Elliptic Curves in Python

For a variety of reasons that will be made clear later, we are not interested in the curve itself, but specific points on the curve. For example, in the curve y^2 = x^3 + 5x + 7, we are interested in the coordinate (-1,1). We are thus going to define the class `Point` to be a point on a specific curve. The curve has the form y^2 = x^3 + ax + b, so we can define the curve with just the numbers a and b.

#### Exercise 1

#### Exercise 2

Write the `__ne__` method for `Point`.

```python
def __ne__(self, other):
    if other is None:
        return True
    return self.num != or self.prime != other.prime
```

### Point Addition

Elliptic curves are useful because of something called point addition. Point addition is where we can do an operation on two of the points on the curve and get a third point, also on the curve.

The way we define point addition is as follows. It turns out that for every elliptic curve, a line will intersect it either one point or three points, except in a couple of special cases.

The two exceptions are when a line is exactly vertical and when a line is tangent to the curve.

We will come back to these two cases later.

We can define point addition using the fact that lines intersect one or three times with the elliptic curve. Two points define a line, so since that line must intersect the curve on more time, that third point reflected over the x-axis is the result of the point addition.

One of the properties we are going to use is that point addition is not easily predictable. We can calculate point addition easily enough with a formula, but intuitively, the result of point addition can be almost anywhere given two points on the curve. 

### Math of Point Addition

Point addition satisfies certain properties that we associate with addition, such as:

- Identity
- Commutativity
- Associativity
- Invertibility

Identity means there's a zero. That is, there exists some point I that, when added to a point A, results in A:

I + A = A

We'll call this point the _point at infinity_ (reasons for this will become clear in a moment).

This is related to _invertibility_. For some point A, there's some other point -A that results in the identity point. That is:

A + (-A) = I

This is why we call this point the point at infinity. We have one extra point in the elliptic curve, which makes the vertical line intersect the curve a third time.

_Commutativity_ means that A + B = B + A. This is obvious since the line going through A and B will intersect the curve a third time in the same place, no matter the order. 

_Associativity_ means that (A + B) + C = A + (B + C). This isn't obvious and is the reason for flipping over the x-axis. 

To code point addition, we're going to split it up into three steps:

1. Where the points are in a vertical line or using the identity point.
2. Where the points are not in a vertical line, but are different.
3. Where the points are the same. 

### Coding Point Addition

We first handle the identity point, or point at infinity. Since we can't easily use infinity in Python, we'll use the None value instead. 

#### Exercise 3

Handle the case where the two points are additive inverses (that is, they have the same x but a different y, causing a vertical line). This should return the point at infinity. 

```python
def __add__(self, other):
    if self.x == other.x and self.y != other.y):
        return self.__class__(None, None, self.a, self.b)
```

### Point Addition for when x_1 != x_2

Now that we've covered the vertical line, let's examine when the points are different. When we have points where the x's differ, we can add using a fairly simple formula. To help with intuition, we'll first find the slope created by the two points. 

```
s = (y_2 - y_1)/(x_2 - x_1)
```

This is the slope, and we can use the slope to calculate x_3. Once we know x_3, we can calculate y_3. P_3 can be derived using this formula:

```
x_3 = s^2 - x_1 - x_2
y_3 = s(x_1 - x_3) - y_1
```

Remember that y_3 is the reflection over the x-axis.

#### Exercise 4

For the curve y^2 = x^3 + 5*x + 7, what is (2,5)+(-1,-1)

### Coding Point Addition for when x_1 != x_2

#### Exercise 5

### Point Addition for when P1 = P2

When the x coordinates are the same and the y coordinate is different, we have the situation where the points are opposite each other over the x-axis. We know that this means:

```
P_1 = -P_2 or P_1 + P_2 = I
```

We've already handled this in Exercise 3.

What happens when P_1 = P_2? Visually, we have to calculate the line that's tangent to the curve at P_1 and find the point at which the line intersects the curve. The situation looks like Figure 2-18, as we saw before. 

#### Exercise 6

#### Exercise 7

### Coding One More Exception

There is one more exception, and this involves the case where the tangent line is vertical.

This can only happen if P1 = P2 and the y coordinate is 0, which case the slope calculation will end up with a 0 in the denominator. 

We handle this with a special case:



