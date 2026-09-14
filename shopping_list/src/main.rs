use std::io;
use std::io::Write;

fn main() {
    let mut input = String::new();
    let mut list: Vec<String> = Vec::new();

    io::stdout().write_all(b"Enter your shopping list, one item at a time.\n").unwrap();
    loop {
        // read a line of input from the terminal
        io::stdin().read_line(&mut input).expect("Failed to read line");

        // break if it's "done". otherwise, add it to list
        if input.trim() == "done"{
            break;
        }
        
        list.push(input.trim().to_string());
        input.clear();
    }

    for item in list{
        println!("* {}", item);
    }
    
    // print list
}
