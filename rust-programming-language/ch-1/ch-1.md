## Chapter 1: Getting Started

### Installation

We'll download Rust trough `rustup`, a command line tool for managing Rust versions and associated tools. 

### Hello, World!

#### Writing and Running a Rust program

```
fn main()
    {
        println!("Hello, World!");
    }
```

Save the file and go back to your terminal window. Enter the following commands to compile and run the file.

```
$ rustc main.rs
$ ./main
Hello, World!
```

#### Anatomy of a Rust Program

`println!` calls a Rust macro. If it called a function instead, it would be entered as `println` (without the `!`). We'll discuss Rust macros in more detail in Chapter 19. For now, you just need to know that using a `!` means that you're calling a macro instead of a normal function, and that macros don't always follow the same rules as functions. 

#### Compiling and Running Are Seperate Steps

You've just run a newly created program, so let's examing each step in the process. 

Before running a Rust program, you must compile it using the Rust compiler by entering the `rustc` command and passing it the name of your source file, like this:

```
$ rustc main.rs
```

You'll notice that this is similar to how C/C++ uses `gcc` or `clang`. After compiling succesfully, Rusts outputs a binary executable. 

```
$ block
main main.rs
```

Rust is an _ahead-of-time compiled_ language, meaning you can run it even without having Rust installed. If you give someone a .rb, .py, or .js file, they need to have a Ruby, Python, or JavaScript implementation installed. But in those languages, you only need one command to compile and run your program. Everything is a trade-off. 

Just compiling with `rustc` is fine for simple programs, but as your project grows, you'll want to manage all the options and make it easy to share your code. Next, we'll introduce you to the Cargo tool, which will help you write real-world Rust programs. 

### Hello, Cargo!

Cargo is Rust's build system and package manager. Cargo handles a lot of tasks for you, such as building your code, downloading the libraries your code depends on, and building those libraries. (we call the libraries that your code needs _dependencies_.)

#### Creating a Project with Cargo

Let's create a new project using Cargo and look how it differs from your original "Hello, world!" project. 

```
$ cargo new hello_cargo
$ cd hello_cargo
```

Go into the _hello_cargo_ directory and list the files. You'll see that Cargo has generated two files and one directory for us: a _Cargo.toml_ file and a _src_ directory with a _main.rs_ file inside.

It has also initialized a new Git repository along with a _.gitignore_ file. 

The last line, `[dependencies]`, is the start of a section for you to list any of your project's dependencies. In Rust, packages of code are referred to as _crates_.

The top-level project directory is just for README files, license information, configuration files, and anything else not related to your code. 

#### Building and Running a Cargo Project

Build your project:

```
$ cargo build
```

You can run the executable with this command:

```
$ ./target/debug/hello_cargo
```

Running `cargo build` for the first time also causes Cargo to create a new file at the top level: _Cargo.lock_. This file keeps track of the exact versions of dependencies in your project. 

We just built a project with `cargo build` and ran it with `./target/debug/hello_cargo`, but we can also use `cargo run` to compile the code and then run the resulting executable all in one command:

```
$ cargo run
```

Cargo also provides a command called `cargo check`. This command quickly checks your code to make sure it compiles but doesn't produce an executable.

#### Building for Release

When your project is finally ready for release, you can use `cargo build --release` to compile with optimizations. This command will create an executable in _target/release_ instead of _target/debug_. 

#### Cargo as Convention

