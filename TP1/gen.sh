for y in 10 20 21 22 23 24 25 30 40 50 100 2000 10000 50000; do
    for x in {1..5}; do 
    python3 gen.py $y Data/$y-$x.txt; 
    done
done