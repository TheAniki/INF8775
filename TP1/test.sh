for y in 10 20 21 22 23 24 25 30 40 50 100 2000 10000 50000; do
    echo $y: &&
    ./tp.sh -a brute -e ./Data/$y-1.txt -t && ./tp.sh -a recursif -e ./Data/$y-1.txt -t
done