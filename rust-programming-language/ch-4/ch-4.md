## Chapter 4: Understanding Ownership

It enables Rust to make memory safety guarantess without needing a garbage collector.

### What is ownership?

Ownership is a set of rules that governs how a Rust program manages memory. All programs have to manage the wat they use a computer's memory while running. Some languages have garbage collection that constantly looks for no-longer used memory as the program runs; in other languages, the programmer must explicitly allocate and free the memory. Rust uses a third approach: memory is managed through a system of ownership with a set of rules that the compiler checks. If any of the rules are violatedm the program won't compile. None of the features of ownership will slow down your program while it's running.

In this chapter, you'll learn ownership by working through some examples that focus on a very common data structure: strings.

#### The stack and the heap

In a systems programming language like Rust, whether a value is on the stack of the heap affects how the language behaves and why you have to make certain decisions. Parts of ownership will be described in relation to the stack and the heap later in this chapter.

Both the stack and the heap are parts of memory available to your code to use at runtime, but they are structured in different ways. The stack stores values in the order it gets them and removes the values in the opposite order. This is referred to as last in, first out. Adding data is called pushing onto the stack, and removing data is called popping off the stack. All data stored on the stack must have a known, fixed size. Data with an unknown size at compile time or a size that might change must be stored on the heap instead.

The heap is less organized: when you put data on the heap, you request a certain amount of space. The memory allocator finds an empty spot in the heap that is big enough, marks it as being in use, and returns a pointer, which is the address of that location. This process is called allocating on the heap and is sometimes abbreviated as just allocating. Pushing values onto the stack is not considered allocating. Because the pointer to the heap is a known, fixed size, you can store the pointer on the stack, but when you want the actual data, you must follow the pointer. 

Pushing to the stack is faster than allocating on the heap because the allocator never has to search for a place to store new data; that location is always at the top of the stack. Comparitely, allocating space on the heap requires more work, because the allocator must first find a big enough space to hold the data and then perform bookkeeping to prepare the next allocation.

Accessing data in the heap is slower that accessing data on the stack because you have to follow a pointer to get there. Contemporary processors are faster if they jump around less in memory. 

When your code calls a function, the values passed into the function (including, potentially, pointers to data on the heap) and the function's variables get pushed onto the stack. When the function is over, those values get popped off the stack.

Keeping track of what parts of code are using what data on the heap, minimizing the amount of duplicate data on the heap, and cleaning up unused data on the heap so you don't run out of space are all problems that ownership addresses. Once you understand ownership, but knowing what the main purpose of ownership is to manage heap data can help explain why it works the way it does.

### Ownership rules

- Each value in Rust has a variable that's called its owner.
- There can only be one owner at a time.
- When the owner goes out of scope, the value will be dropped.

### Variable scope

### The string type

The types covered previously are all a known size, can be stored on the stack and popped off the stack when their scope is over, and can be quickly and trivially copied to make a new, independent instance if another part of code needs to use the same value in a different scope. But we want to look at data that is stored on the heap and explore how Rust knowns when to clean up that data, and the `String` type is a great example.

We've already seen string literals, where a string value is hardcoded into our program. String literals are convenient, but they aren't suitable for every situation in which we may want to use text. One reason is that they're immutable. Another is that not every string value can be known when we write our code: for example, what if we want to take user input and store it? For these situations, Rust has a second string type, `String`. This type manages data allocated on the heap and as such is able to store an amount of text that is unknown to us at compile time. You can create a `String` from a string literal using the `from` function, like so:

```rust
let s = String::from("hello");
```

The double colon `::` operator allows us to namespace this particular `from` function under the `String` type.

This kind of string can be mutated:

```rust
let mut s = String::from("Hello");

s.push_str(" , world!");    // push_str() appends a literal to a String

println!("{}", s);  
```

Why can `String` be mutated but literals cannot? The difference is how these two types deal with memory.

### Memory and allocation

In the case of a string literal, we know the contents at a compile time, so the text is hardcoded directly into the final executable. This is why string literals are fast and efficient. But these properties only come from the string literal's immutability. 

With the `String`

continue here: https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html