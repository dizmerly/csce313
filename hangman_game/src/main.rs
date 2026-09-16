extern crate rand;

use rand::prelude::*;
use std::fs;
use std::io;
use std::io::Write;

const NUM_INCORRECT_GUESSES: u32 = 5;
const WORDS_PATH: &str = "words.txt";

fn pick_a_random_word() -> String {
  let file_string = fs::read_to_string(WORDS_PATH).expect("Unable to read file.");
  let words: Vec<&str> = file_string.split('\n').collect();
  String::from(words[rand::rng().random_range(0..words.len())].trim())
}

fn print_chars(ch: &Vec<char>) {
  // print all the characters in the vector
  let mut output = String::new();
  for c in ch{
      output.push(*c);
  }
  println!("{}", output);
}

fn num_valid_chars(ch: &Vec<char>) -> usize {
  // number of characters in the vector that are not '_'
  ch.iter().filter(|c| **c != '_').count()
}

fn main() {
  let secret_word = pick_a_random_word();
  let secret_word_chars: Vec<char> = secret_word.chars().collect();
  println!("random word: {}", secret_word);

  let mut current_guess = vec!['_'; secret_word_chars.len()];
  let mut guessed_letters: Vec<char> = Vec::new();
  let mut guesses_left = NUM_INCORRECT_GUESSES;

  while guesses_left > 0 {
      // print the characters in the current guess
      print_chars(&current_guess);   
      // print # of guesses left
      println!("Guesses left: {}", guesses_left);

      print!("Please guess a letter: ");
      io::stdout().flush().unwrap();

      let mut input = String::new();
      io::stdin().read_line(&mut input).expect("Failed to read line");
      let input_char = input.chars().next().unwrap();
      
      // Check whether this letter has already been guessed.
      if guessed_letters.contains(&input_char) {
          println!("You already guessed that letter.");
          continue;
      }
      
      guessed_letters.push(input_char);
      
      // Reveal every occurrence of the guessed letter.
      let mut correct_guess = false;
      
      for i in 0..secret_word_chars.len() {
          if secret_word_chars[i] == input_char {
              current_guess[i] = input_char;
              correct_guess = true;
          }
      }
      
      if correct_guess {
          println!("Correct!");
          if num_valid_chars(&current_guess) == secret_word_chars.len() {
              print_chars(&current_guess);
              println!("You win!");
              return;
          }
      } else {
          println!("Incorrect!");
          guesses_left -= 1;
      }

  }
  // print failure message
  println!("You lose!")
}