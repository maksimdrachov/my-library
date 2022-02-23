## Chapter 1: Getting Started

Second, `println!` calls a Rust macro. If it called a function instead, it would be entered as `println` (without the `!`). For now, you just need to know that using a `!` means that you're calling a macro instead of a normal function, and that macros don't always follow the same rules as functions.

The last line, `[dependencies]`, is the start of a section for you to list any of your project's dependencies. In Rust, packages of code are referred to as _crates_. 

Cargo expects your source files to live inside the _src_ directory. The top-level project directory is just for README files, licence information, configuration files, and anything else not related to your code.

- build project: `cargo build`
- compile and run code: `cargo run`
- checks if code compiles: `cargo check`
- build for release: `cargo build --release`
- 