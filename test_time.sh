#!/bin/zsh

# 実行するC++ファイルを引数で指定（デフォルトは main.cpp）
cpp_file=${1:-main.cpp}

# コンパイル済みバイナリの名前
binary="./a.out"

# 合計スコアの初期化
sum=0
max_time=0  # 最大実行時間を記録する変数

# C++ファイルのコンパイル
echo "Compiling $cpp_file..."
g++ -I. -std=c++23 "$cpp_file" -o $binary -O2

# コンパイルに失敗した場合のエラーハンドリング
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# テストケースの実行
for f in $(ls ./tools/in); do
    echo "Testing $f"

    # 開始時刻を記録
    start_time=$(gdate +%s%N)
    
    # プログラムの実行
    ($binary < ./tools/in/$f) > ./out
    
    # 終了時刻を記録
    end_time=$(gdate +%s%N)
    
    # 実行時間を計算（ナノ秒をミリ秒に変換）
    elapsed_time=$(( (end_time - start_time) / 1000000 ))
    echo "Execution Time: ${elapsed_time} ms"

    # 最大時間を更新
    if [ $elapsed_time -gt $max_time ]; then
        max_time=$elapsed_time
    fi

    # スコアを取得
    res=$(./tools/target/release/vis ./tools/in/$f ./out)
    echo $res
    sum=$((sum + res))
done

# 合計スコアにカンマを付ける
formatted_sum=$(printf "%'d" $sum)
average=$((sum / 100))
formatted_average=$(printf "%'d" $average)

# 合計スコアと100で割った結果を出力
echo "Total Score: $formatted_sum"
echo "Score divided by 100: $formatted_average"

# 最大実行時間を出力
echo "Maximum Execution Time: ${max_time} ms"

