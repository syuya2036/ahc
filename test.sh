sum=0
g++ -I. -std=c++23 main.cpp
for f in $(ls ./tools/in); do
    echo "Testing $f"
    (./a.out < ./tools/in/$f) > ./out
    res=$(./tools/target/release/vis ./tools/in/$f ./out)
    echo $res
    sum=$((sum + res))
done

echo "Score: $sum"
