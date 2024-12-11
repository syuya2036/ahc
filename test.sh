#!/bin/zsh

# 実行するC++ファイルを引数で指定（デフォルトは main.cpp）
cpp_file=${1:-main.cpp}

# コンパイル済みバイナリの名前
binary="./a.out"

# 合計スコアの初期化
sum=0
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
    ($binary < ./tools/in/$f) > ./out
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
