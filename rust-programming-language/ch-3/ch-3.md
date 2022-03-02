## Chapter 3: Common Programming Concepts

### Variables and Mutability

By default variables are immutable.

You can make them mutable by adding `mut` in front of the variable name.

There are multiple trade-offs to consider in addition to the prevention of bugs. For example, in cases where you're using large data structures, mutating an instance in place may be faster than copying and returning newly allocated instances. With smaller data structures, creating new instances and writing in a more functional programming style may be easier to think through, so lower performance might be a worthwile penalty for gaining that clarity.

#### Shadowing

Shadowing is different from marking a variable as `mut`, because we'll get a compile-time error if we accidentally try to reassign to this variable without using the `let` keyword. By using `let`, we can perform a few transformations on a value but have the variable be immutable after thos transformation have been completed.

The other difference between `mut` and shadowing is that because we're effectively creating a new variable when we use the `let` keyword again, we can change the type of the value but reuse the same name.

### Data Types

Compound types can group multiple values into one type. Rust has two primitive compound types: tuples and arrays.

A tuple is a general way of grouping together a number of values with a variety of types into one compound type. Tuples have a fixed length: once declared, they cannot grow or shrink in size.

Arrays in Rust have a fixed length.

Continue here: https://doc.rust-lang.org/stable/book/ch03-03-how-functions-work.html