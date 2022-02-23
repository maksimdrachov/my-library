## Chapter 2: Programming a Guessing Game

THe `::` syntax in the `::new` line indicates that `new` is an associated function of the `String` type. An _associated function_ is a function that's implemented on a type, in this case `String`. 

Next, the line `.read_line(&mut guess)` calls the `read_line` method on the standard input handle to get input from the user. We're also passing `&mut guess` as the argument to `read_line` to tell it what string to store the user input in. The string argument needs to be mutable so the method can change the string's content.

The `&` indicates that this argument is a _reference_, which gives you a way to let multiple parts of your code access one piece of data without needing to copy that data into memory multiple times. For now, all you need to know is that like variables, references are immutable by default. Hence, you need to write `&mut guess` rather than `&guess` to make it mutable. 

As mentioned earlier, `read_line` puts whatever the user enters into the string we pass to it, but it also returns a value--in this case, an `io::Result`. Rust has a number of types named `Result` in its standard library: a generic `Result` as well as specific versions for submodules, such as `io::Results`. The `Result` types are _enumerations_, often referred to as _enums_, which can have a fixed set of possibilities known as _variants_. Enums are often used with `match`, a conditional that makes it convenient to execute different code based on which variant an enum value is when the conditional is evaluated. 

`Result`'s variants are `Ok` or `Err`. The `Ok` variant indicates the operation was succesful, and inside `Ok` is the succesfully generated value. The `Err` variant means the operation failed, and `Err` contains information about how or why the operation failed. 

Values of the `Result` type, like values of any type, have methods defined on them. An instance of `io::Result` has an `expect` method that you can call. If this instance of `io::Result` is an `Err` value, `expect` will cause the program to crash and display the message that you passed as an argument to `expect`. If the `read_line` method returns an `Err`, it would likely be the result of an error coming from the underlying operating system. If this instance of `io::Result` is an `Ok` value, `expect` will take the return value that `Ok` is holding and return just that value to you so you can use it. In this case, that value is the number of bytes in the user's input. 

In the first line, we call the `rand::thread_rng` function that gives us the particular random number generator that we're going to use: one that is local to the current thread of execution and seeded by the operating system. Then we call the `gen_range` method on the random number generator. This method is defined by the `Rng` trait that we brought into scope with the `use rand::Rng` statement. 

Another neat feature of Cargo is that running the `cargo doc --open` command will build documentation provided by all of your dependencies locally and open it in your browser. 

Helpfully Rust allows us to _shadow_ the previous value of `guess` with a new one. Shadowing lets us reuse the `guess` variable name rather than forcing us to create two unique variables, such as `guess_str` and `guess` for example. This feature is often used when you want to convert a value from one type to another type.

We bind this new variable to the expression `guess.trim().parse()`. The `guess` in the expression refers to the original `guess` variable that contained the input as string. The `trim` method on a `String` instance will eliminate any whitespace at the beginning and end, which we must do to be able to compare the string to the `u32`, which can only contain numerical data. The user must press enter to satisfy `read_line` and input their guess, which adds a newline character to the string. 

The `parse` method on strings parses a string into some kind of number. Because this method can parse a variety of number types, we need to tell Rust the exact number of type we want by using `let guess: u32`. The colon (:) after guess tells Rust we'll annotate the variable's type. Rust has a few built-in number types; the `u32` seen here is an unsigned, 32-bit integer. It's a good default choice for a small positive number. You'll learn about other number types in Chapter 3. 

The underscoer, `_` is a catchall value; in this example, we're saying we want to match all `Err` values, no matter what information they have inside them. So the program will execute the second arm's code, `continue`, which tells the program to go to the next iteration of the `loop` and ask for another guess. 