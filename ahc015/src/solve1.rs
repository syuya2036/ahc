use ac_library::dsu::Dsu;
use proconio::{input, source::line::LineSource};
use std::{fmt::Debug, io::{stdin, stdout, BufReader, Write}, process::exit};

#[derive(Debug, Clone)]
pub struct Board {
    pub board: Vec<Vec<u8>>,
    ft: Vec<u8>,
}

impl std::fmt::Display for Board {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        for h in 0..10 {
            for w in 0..10 {
                write!(f, "{}", self.board[h][w])?;
            }
            writeln!(f)?;
        }
        Ok(())
    }
}

impl Board {
    pub fn new(ft: Vec<u8>) -> Self {
        Self {
            board: vec![vec![0; 10]; 10],
            ft,
        }
    }

    pub fn put(&mut self, x: usize, y: usize, ft: u8) {
        assert!(self.board[y][x] == 0);
        self.board[y][x] = ft;
    }

    pub fn swap(&mut self, y: usize, x: usize, toy: usize, tox: usize) {
        let t = self.board[y][x];
        self.board[y][x] = self.board[toy][tox];
        self.board[toy][tox] = t;
    }

    pub fn op_forward(&mut self) {
        for w in 0..10 {
            let mut cnt = 0;
            for h in 0..10 {
                if self.board[h][w] != 0 {
                    self.swap(cnt, w, h, w);
                    cnt += 1;
                }
            }
        }
    }

    pub fn op_backward(&mut self) {
        for w in 0..10 {
            let mut cnt = 9;
            for h in (0..10).rev() {
                if self.board[h][w] != 0 {
                    self.swap(cnt, w, h, w);
                    if cnt == 0 {
                        break;
                    }
                    cnt -= 1;
                }
            }
        }
    }

    pub fn op_right(&mut self) {
        for h in 0..10 {
            let mut cnt = 9;
            for w in (0..10).rev() {
                if self.board[h][w] != 0 {
                    self.swap(h, cnt, h, w);
                    if cnt == 0 {
                        break;
                    }
                    cnt -= 1;
                }
            }
        }
    }

    pub fn op_left(&mut self) {
        for h in 0..10 {
            let mut cnt = 0;
            for w in 0..10 {
                if self.board[h][w] != 0 {
                    self.swap(h, cnt, h, w);
                    cnt += 1;
                }
            }
        }
    }

    pub fn rule_based_playout(&self, t_start: usize) -> u8 {
        assert!(t_start < 100);

        let mut best_score = 0;
        let mut best_dir = 0;
        for d in 0..4 {
            let mut new_board = self.clone();
            let mut mean_score = 0;
            let mut simulate_cnt = 0;
            match d {
                0 => new_board.op_forward(),
                1 => new_board.op_backward(),
                2 => new_board.op_right(),
                3 => new_board.op_left(),
                _ => unreachable!(),
            }

            let time_keeper = TimeKeeper::new(0.085 / (100 - t_start) as f64);

            while !time_keeper.is_time_over() {
                let mut playout_board = new_board.clone();

                for t in (t_start)..100 {
                    let random_legal_place = playout_board.random_legal_place();
                    playout_board.put(random_legal_place.0, random_legal_place.1, self.ft[t]);

                    match self.ft[t] {
                        1 => playout_board.op_right(),
                        2 => playout_board.op_left(),
                        3 => playout_board.op_forward(),
                        _ => (),
                    }
                }

                mean_score += playout_board.score();
                simulate_cnt += 1;
            }

            mean_score /= simulate_cnt;
            if mean_score > best_score {
                best_score = mean_score;
                best_dir = d;
            }
        }

		println!("best_score: {}", best_score);
        best_dir
    }

    pub fn random_legal_place(&self) -> (usize, usize) {
        let legal_places = self.get_legal_place();
        let idx = rnd::gen_range(0, legal_places.len());
        legal_places[idx]
    }

    pub fn get_legal_place(&self) -> Vec<(usize, usize)> {
        let mut legal_places = vec![];
        for h in 0..10 {
            for w in 0..10 {
                if self.board[h][w] == 0 {
                    legal_places.push((w, h));
                }
            }
        }

        legal_places
    }

    pub fn score(&self) -> usize {
        let mut uf = Dsu::new(100);
        let dx = vec![1, 0, -1, 0];
        let dy = vec![0, 1, 0, -1];
        for h in 0..10 {
            for w in 0..10 {
                if self.board[h][w] == 0 {
                    continue;
                }
                for d in 0..4 {
                    let nh = h as i32 + dy[d];
                    let nw = w as i32 + dx[d];
                    if nh < 0 || nh >= 10 || nw < 0 || nw >= 10 {
                        continue;
                    }
                    if self.board[h][w] == self.board[nh as usize][nw as usize] {
                        uf.merge(h * 10 + w, nh as usize * 10 + nw as usize);
                    }
                }
            }
        }

        let mut score = 0;
        for g in uf.groups() {
            if g[0] == 0 {
                continue;
            }
            score += g.len() * g.len();
        }

        score
    }
}

pub fn solve() {
    let stdin = stdin();
    let mut source = LineSource::new(BufReader::new(stdin.lock()));

    input! {
        from &mut source,
        ft: [u8; 100],
    }

    let mut board = Board::new(ft.clone());

    for t in 0..99 {
        input! {
            from &mut source,
            c: usize,
        }

        let legal_places = board.get_legal_place();
        let (x, y) = legal_places[c - 1];
        board.put(x, y, ft[t]);
        let best_dir = board.rule_based_playout(t + 1);

        match best_dir {
            0 => {
                println!("F");
                board.op_forward();
            },
            1 => {
                println!("B");
                board.op_backward();
            },
            2 => {
                println!("R");
                board.op_right();
            },
            3 => {
                println!("L");
                board.op_left();
            },
            _ => unreachable!(),
        }

        stdout().flush().unwrap();
    }
}

#[derive(Debug, Clone)]
struct TimeKeeper {
    start_time: std::time::Instant,
    time_threshold: f64,
}

impl TimeKeeper {
    fn new(time_threshold: f64) -> Self {
        TimeKeeper {
            start_time: std::time::Instant::now(),
            time_threshold,
        }
    }

    #[inline]
    fn is_time_over(&self) -> bool {
        let elapsed_time = self.start_time.elapsed().as_nanos() as f64 * 1e-9;
        #[cfg(feature = "local")]
        {
            elapsed_time * 0.85 >= self.time_threshold
        }
        #[cfg(not(feature = "local"))]
        {
            elapsed_time >= self.time_threshold
        }
    }
}

mod rnd {
    use rand::Rng;
    static mut S: usize = 0;
    static MAX: usize = 1e9 as usize;

    #[inline]
    pub fn init(seed: usize) {
        unsafe {
            if seed == 0 {
                S = rand::thread_rng().gen();
            } else {
                S = seed;
            }
        }
    }
    #[inline]
    pub fn gen() -> usize {
        unsafe {
            if S == 0 {
                init(0);
            }
            S ^= S << 7;
            S ^= S >> 9;
            S
        }
    }

    #[inline]
    pub fn gen_range(a: usize, b: usize) -> usize {
        if a == b {
            return a;
        }
        gen() % (b - a) + a
    }
    #[inline]
    pub fn gen_bool() -> bool {
        gen() & 1 == 1
    }
    #[inline]
    pub fn gen_float() -> f64 {
        ((gen() % MAX) as f64) / MAX as f64
    }
}


fn main() {
    solve();
}
