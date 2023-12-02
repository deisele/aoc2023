#![allow(non_snake_case)]

use std::fs::File;
use std::io::{BufRead, BufReader};

fn main() {
    // part 1
    let inputReader = BufReader::new(File::open("src/input").unwrap());
    let inputLines = inputReader.lines().map(|x| x.unwrap());

    let mut sum: u32 = 0;
    'game: for line in inputLines {
        let a: Vec<_> = line.split(": ").collect();
        let gameId = a[0].split(" ").last().unwrap().parse::<u32>().unwrap();

        let draws: Vec<_> = a[1].split("; ").collect();
        for draw in draws {
            let cubeList: Vec<_> = draw.split(", ").collect();
            for cube in cubeList {
                let b: Vec<_> = cube.split(" ").collect();
                
                if (b[1] == "red" && b[0].parse::<u32>().unwrap() > 12) || (b[1] == "green" && b[0].parse::<u32>().unwrap() > 13) || (b[1] == "blue" && b[0].parse::<u32>().unwrap() > 14) {
                    continue 'game;
                }
            }
        }

        sum += gameId;
    }

    println!("Sum 1: {}", sum);

    // part 2
    let inputReader = BufReader::new(File::open("src/input").unwrap());
    let inputLines = inputReader.lines().map(|x| x.unwrap());

    let mut sum: u32 = 0;
    'game: for line in inputLines {
        let mut minRed: u32 = 0;
        let mut minGreen: u32 = 0;
        let mut minBlue: u32 = 0;

        let a: Vec<_> = line.split(": ").collect();
        let gameId = a[0].split(" ").last().unwrap().parse::<u32>().unwrap();

        let draws: Vec<_> = a[1].split("; ").collect();
        for draw in draws {
            let cubeList: Vec<_> = draw.split(", ").collect();
            for cube in cubeList {
                let b: Vec<_> = cube.split(" ").collect();

                let color = b[1];
                let amount = b[0].parse::<u32>().unwrap();
                
                if color == "red" && amount > minRed {
                    minRed = amount;
                } else if color == "green" && amount > minGreen {
                    minGreen = amount;
                } else if color == "blue" && amount > minBlue {
                    minBlue = amount;
                }
            }
        }

        sum += minRed * minGreen * minBlue;
    }

    println!("Sum 2: {}", sum);
}
