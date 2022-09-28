for y in 10 50 100 500 1000 5000 10000; do
    for x in {1..5}; do 
    python3 gen.py $y Data/$y-$x.txt; 
    done
done