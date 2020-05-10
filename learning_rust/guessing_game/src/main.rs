use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn main() {
    println!("Alright, tough guy.");

    let secret_number = rand::thread_rng().gen_range(1, 101);

    loop {
        println!("Enter guess.");

        let mut guess = String::new();

        io::stdin()
            .read_line(&mut guess)
            .expect("Failure is an option");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Type a *number* you pleeb!");
                continue;
            }
        };

        match guess.cmp(&secret_number) {
            Ordering::Greater => println!("This guess is too large."),
            Ordering::Less => println!("This guess is too small."),
            Ordering::Equal => {
                println!("This guess is just right.");
                break;
            }
        }
    }
}
