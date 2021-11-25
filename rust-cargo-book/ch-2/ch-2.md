### Programming a Guessing Game

```
use std::io;
```

By default, Rust brings only a few types into the scope of every program in the _prelude_. If a type you want to use isn't in the prelude, you have to bring that type into scope explicitly with a `use` statement. Using the `std::io` library provides you with a number of useful features, including the ability to accept user input. 

`println!` is a macro that prints a string to the screen:

```
println!("Guess the number!");
```

#### Storing Values with Variables

```rust
let mut guess = String::new(); 
```

In Rust, variables are immutable by default. The following example shows how to use `mut` before the variable name to make a variable mutable:

```rust
let apples = 5; // immutable
let mut bananas = 5; // mutable
```

The value that `guess` is bound to, which is the result of calling `String::new`, a function that returns a new instance of a `String`. `String` is a string type provided by the standard library that is a growable, UTF-8 encoded bit of text. 

The `::` syntax in the `::new` line indicates that `new` is an _associated function_ of the `String` type. An associated function is implemented on a type, in this case `String`.

Recall that we included the input/output functionality from the standard library with `use std::io`; on the first line of the program. Now we'll call the `stdin` function from the `io` module:

```
io::stdin()
    .read_line(&mut guess)
```

