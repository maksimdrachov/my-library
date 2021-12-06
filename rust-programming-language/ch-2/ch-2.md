## Chapter 2: Programming a Guessing Game

You'll learn about:
- `let`
- `match`
- methods
- associated functions
- using external crates
- and more

### Setting Up a New Project

```
cargo new guessing_game
cd guessing_game
```

```
$ cargo run
```

The `run` command comes in handy when you need to rapidly iterate on a project, as we'll do in this game, quickly testing each iteration before moving on to the next one.

### Processing a Guess

- ask for user input
- process that input
- check that the input is in the expected form

If a type you want to use isn't in the prelude, you have to bring that type into scope explicitly with a `use` statement. Using the `std::io` library provides you with a number of useful features, including the ability to accept user input.

### Storing Values with Variables

Next, we'll create a place to store the user input:

```rust
let mut guess = String::new(); 
```

A `let` statement, which is used to create a _variable_. 

In Rust, variables are immutable by default. We'll be discussing this concept in detail in the "Variables and Mutability" section in Chapter 3. Use `mut` before the variable to make a variable mutable. 

`String::new` returns a new instance of a `String`. `String` is a string type provided by the standard library that is growable, UTF-8 encoded bit of text.

The `::` syntax indicates that `new` is an _associated function_ of the `String` type.

Now we'll call the `stdin` function from the `io` module:

```
io::stdin()
    .read_line($mut guess)
```

.read_line(&mut guess), calls the read_line method on the standard input handle to get input from the user. We're also passing one argument to `read_line`: `&mut guess`.

The `&` indicates that this argument is a _reference_, which gives you a way to let multiple parts of your code access one piece of data without needing to copy that data into memory multiple times. References are a complex feature, and one of Rusts's major advantages is how sage and easy it is to use references. For now all you need to know is that like variables, references are immutable by default. Hence, you need to write `&mut guess` rather than `&guess` to make it mutable. 

### Handling Potential Failure with the Result Type

```
    .expect("Failed to read line");
```

As mentioned earlier, `read_line` puts what the user types into the string we're passing it, but it also returns a value-in this case, an `io::Result`. Rust has a number of types named `Result` in its standard library: a generic `Result` as well as specific versions for submodules, such as `io::Result`.

The Result