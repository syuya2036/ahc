use std::fs;
use std::io::{self, Write};
use std::process::{Command, Stdio};
use std::sync::{Arc, Mutex};
use rayon::prelude::*;
use num_format::{Locale, ToFormattedString};
use indicatif::{ProgressBar, ProgressStyle};
use std::{thread, time};

fn main() -> io::Result<()> {
    // 引数からC++ファイルを取得 (デフォルトは main.cpp)
    let args: Vec<String> = std::env::args().collect();
    let default_file_name = "main.cpp".to_string();
    let cpp_file = args.get(1).unwrap_or(&default_file_name);

    // コンパイル済みバイナリの名前
    let binary = "./a.out";

    // C++ファイルをコンパイル
    println!("Compiling {}...", cpp_file);
    let compile_status = Command::new("g++")
        .args(&["-I.", "-std=c++23", cpp_file, "-o", binary, "-O2"])
        .status()?;

    if !compile_status.success() {
        eprintln!("Compilation failed.");
        std::process::exit(1);
    }

    // テストケースのパスを取得
    let input_dir = "./tools/in";
    let inputs: Vec<_> = fs::read_dir(input_dir)?
        .filter_map(|entry| entry.ok())
        .map(|entry| entry.path())
        .collect();

    let output_dir = "./out";

    // プログレスバーの作成
    let progress_bar = ProgressBar::new(inputs.len() as u64);
    progress_bar.set_style(
        ProgressStyle::default_bar()
            .template("{msg} [{bar:40.cyan/blue}] {pos}/{len} {percent}%")
            .expect("Invalid progress bar template")
            .progress_chars("#>-"),
    );

    // 合計スコアと最大時間を保持する共有変数
    let sum = Arc::new(Mutex::new(0));
    let max_time = Arc::new(Mutex::new(0));

    // テストケースを並列実行
    inputs.par_iter().for_each(|input_path| {
        if let Some(input_name) = input_path.file_name().and_then(|n| n.to_str()) {
            progress_bar.set_message(format!("Testing {}", input_name));
            let output_file = format!("out.{}", input_name);

            // 開始時刻を記録
            let start_time = std::time::Instant::now();

            // プログラムの実行
            let output_status = Command::new(binary)
                .stdin(fs::File::open(input_path).unwrap())
                .stdout(fs::File::create(&output_file).unwrap())
                .status();

            if output_status.is_err() {
                eprintln!("Error running {} on {}", binary, input_name);
                progress_bar.inc(1);
                return;
            }

            // 実行時間を計算
            let elapsed_time = start_time.elapsed().as_millis();
            let mut max_time_lock = max_time.lock().unwrap();
            if elapsed_time > *max_time_lock {
                *max_time_lock = elapsed_time;
            }
            drop(max_time_lock);

            // スコアを取得
            let vis_output = Command::new("./tools/target/release/vis")
                .args(&[input_path.to_str().unwrap(), &output_file])
                .output();

            let _ = Command::new("rm").args(&[output_file]).output().unwrap();
            if let Ok(output) = vis_output {
                if let Ok(score_str) = String::from_utf8(output.stdout) {
                    if let Ok(score) = score_str.trim().parse::<i32>() {
                        let mut sum_lock = sum.lock().unwrap();
                        *sum_lock += score;
                        println!("{} Score = {}", input_name, score);
                    }
                }
            }

            progress_bar.inc(1);
        }
    });

    progress_bar.finish_with_message("All tests completed!");

    // 合計スコアと平均スコアを計算
    let total_score = *sum.lock().unwrap();
    let formatted_total_score = total_score.to_formatted_string(&Locale::en);
    let average_score = total_score / 100;
    let formatted_average_score = average_score.to_formatted_string(&Locale::en);

    // 最大実行時間を取得
    let maximum_time = *max_time.lock().unwrap();

    // 結果を出力
    println!("Total Score: {}", formatted_total_score);
    println!("Score divided by 100: {}", formatted_average_score);
    println!("Maximum Execution Time: {} ms", maximum_time);

    Ok(())
}
