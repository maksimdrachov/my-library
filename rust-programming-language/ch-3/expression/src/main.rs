fn main() {
    let y = 
    {
        let x = 3;
        x + 1
    };

    println!("The value of y is: {}", y);
}

/*

Note that x+1 line does not have a semicolon at the end, unlike most of the lines you've seen so far. 
Expressions do not include ending semicolons.
If you add a semicolon to the end of an expression, you turn it into a statement,
and it will then not return a value.

*/