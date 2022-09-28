for y in 10 25 30 40 50 100 2000 10000 50000; do
    echo $y: &&
    python3 ./Code/main.py -a brute -e ./Data/$y-1.txt -t &&
    python3 ./Code/main.py -a recursif -e ./Data/$y-1.txt -t &&
    python3 ./Code/main.py -a seuil -e ./Data/$y-1.txt -t
done