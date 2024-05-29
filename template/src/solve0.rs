use proconio::{input, source::line::LineSource};
use std::io::{stdin, stdout, BufReader, Write};

pub fn solve() {
    let stdin = stdin();
    let mut source = LineSource::new(BufReader::new(stdin.lock()));

    input! {
        from &mut source,
        ft: [u8; 100],
    }


    for t in 0..100 {
        input! {
            from &mut source,
            c: u8
        }
        if ft[t] == 1 {
            println!("{}", 'R');
        } else if ft[t] == 2 {
            println!("{}", 'L');
        } else {
            println!("{}", 'B');
        }
        stdout().flush().unwrap();
    }
}

fn main() {
    solve();
}
