use std::collections::HashMap;

// Sorts a vector of integers in place.
fn isort(data: &mut Vec<i32>) {
    for i in 0..data.len() {
        for j in i..data.len() {
            if data[j] < data[i] {
                let swap = data[i];
                data[i] = data[j];
                data[j] = swap;
            }
        }
    }
}

// Computes the mean, median, and mode of a vector of integers.
fn stats(data: &Vec<i32>) -> (i32, i32, i32) {
    if data.len() == 0 {
        panic!("stats undefined for empty data");
    }

    // Sum the data, check if the data is sorted, and count the occurance of each unique integer.
    let mut sum = 0;
    let mut prev = data[0];
    let mut is_sorted = true;
    let mut counts = HashMap::new();
    for val in data {
        // Add value to sum.
        sum += val;

        // Check that the data is ordered.
        is_sorted = is_sorted && *val >= prev;
        prev = *val;

        // Count the number of times the value occurs.
        let count = counts.entry(*val).or_insert(0);
        *count += 1;
    }

    // Compute the mean.
    let mean = sum / data.len() as i32;

    // Compute the median, ensuring the data is sorted.
    if !is_sorted {
        panic!("expected data to be sorted");
    }
    let median = data[data.len() >> 1];

    // Compute the mode.
    let mut mode = data[0];
    let mut max_count = 0;
    for (val, count) in &counts {
        if *count > max_count {
            mode = *val;
            max_count = *count;
        }
    }

    (mean, median, mode)
}

// Translates a sentence into pig latin.
fn pigify(s: &String) -> String {
    // Decides if a character is punctuation or belongs to a word.
    fn is_punctuation(c: char) -> bool {
        let ps = vec![' ', '.', ',', ':', ';', '?', '!'];
        for p in ps {
            if p == c {
                return true;
            }
        }
        false
    }

    // Decides if a character is a vowel.
    fn is_vowel(c: char) -> bool {
        let ps = vec!['a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'];
        for p in ps {
            if p == c {
                return true;
            }
        }
        false
    }

    // Converts a word into pig latin.
    fn pigify_word(word: &String) -> String {
        let mut pig_word = String::new();
        let mut iter = word.chars();
        match iter.next() {
            Some(c) => {
                if is_vowel(c) {
                    pig_word.push_str(&word[..]);
                    pig_word.push_str("-hay");
                } else {
                    for d in iter {
                        pig_word.push(d);
                    }
                    pig_word.push('-');
                    pig_word.push(c);
                    pig_word.push_str("ay");
                }
                pig_word
            }
            None => pig_word,
        }
    }

    let mut pig = String::new();
    let mut tok = String::new();
    if s.len() == 0 {
        return pig;
    }

    // Process the sequence of words and punctuation, excluding the last.
    let mut parsing_word = None;
    for c in s.chars() {
        match (parsing_word, is_punctuation(c)) {
            (Some(true), true) => {
                // End of a word.
                pig.push_str(&pigify_word(&tok));
                tok.clear();
                parsing_word = Some(false);
            }
            (Some(false), false) => {
                // End of punctuation.
                pig.push_str(&tok[..]);
                tok.clear();
                parsing_word = Some(true);
            }
            (None, c_is_punc) => parsing_word = Some(!c_is_punc),
            _ => (),
        };
        tok.push(c);
    }

    // Process last word or punctuation.
    if parsing_word == Some(true) {
        pig.push_str(&pigify_word(&tok));
    } else {
        pig.push_str(&tok[..]);
    }

    pig
}

fn main() {
    // pp. 147
    //
    // First exercise.
    let mut data = vec![
        1, 1, 1, 23, 43, 0, -2344, 99, 8293, 0, 0, 1, 1, 1, 23, 3, 58, 107, 234, -9902, -1337,
    ];
    isort(&mut data);
    let (mean, median, mode) = stats(&data);
    println!(
        "First. Mean: {0}; median: {1}; mode: {2}.",
        mean, median, mode
    );

    // Second exercise.
    //let data = String::from("Hohenschönhausen.  Dieser Zug endet hier...bitte alle aussteigen!");
    let data = String::from("this sentence is in pig-latin.");
    println!("Second. {}", pigify(&data));
}
