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
  ch.iter().filter(|c| *c != '_').count()
}

fn main() {
  let secret_word = pick_a_random_word();
  let secret_word_chars: Vec<char> = secret_word.chars().collect();
  println!("random word: {}", secret_word);

  let mut guesses_left = NUM_INCORRECT_GUESSES;

  while guesses_left > 0 {
      // print the characters in the current guess
      print_chars(&secret_word_chars);      
      // print # of guesses left
      println!("Guesses left: {}", guesses_left);

      print!("Please guess a letter: ");
      io::stdout().flush().unwrap();

      let mut input = String::new();
      io::stdin().read_line(&mut input).expect("Failed to read line");
      let input_char = input.chars().next().unwrap();

      // adjust guessed_letters, current guess
      // adjust guesses_left
      guesses_left -= 1;
      // return if successfully guessed word
      return 
  }
  // print failure message
  println!("You lose!")
}