use proconio::{input, source::line::LineSource};
use std::io::{stdin, stdout, BufReader, Write};

pub fn solve() {
    let stdin = stdin();
    let mut source = LineSource::new(BufReader::new(stdin.lock()));

    input! {
        from &mut source,
        ft: [u8; 100],
    }

    for t in 0..99 {
        input! {
            from &mut source,
            c: u8
        }
        let mut out = 'F';

        

        println!("{}", out);

        stdout().flush().unwrap();
    }
}
