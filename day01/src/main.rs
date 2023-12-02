#![allow(non_snake_case)]

use std::fs::File;
use std::io::{BufRead, BufReader};

fn main() {
    let inputReader = BufReader::new(File::open("src/input").unwrap());
    let inputLines = inputReader.lines().map(|x| x.unwrap());

    let mut sum1: u32 = 0;
    let mut sum2: u32 = 0;
    for line in inputLines {
        // part 1
        let digits: Vec<u32> = line.chars().filter_map(|c| c.to_digit(10)).collect();

        sum1 += digits.first().unwrap() * 10 + digits.last().unwrap();

        // part 2

        let line = line
            .replace("one", "one1one")
            .replace("two", "two2two")
            .replace("three", "three3three")
            .replace("four", "four4four")
            .replace("five", "five5five")
            .replace("six", "six6six")
            .replace("seven", "seven7seven")
            .replace("eight", "eight8eight")
            .replace("nine", "nine9nine");

        let digits: Vec<u32> = line.chars().filter_map(|c| c.to_digit(10)).collect();

        sum2 += digits.first().unwrap() * 10 + digits.last().unwrap();
    }

    println!("Sum 1: {}", sum1);
    println!("Sum 2: {}", sum2);
}
