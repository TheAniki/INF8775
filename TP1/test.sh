for y in 10 50 100 500 1000 5000 10000; do
    echo $y: &&
    for x in {1..5}; do
        echo $x ": ================="&&
        ./tp.sh -a brute -e ./Data/$y-$x.txt -t &&
        ./tp.sh -a recursif -e ./Data/$y-$x.txt -t &&
        ./tp.sh -a seuil -e ./Data/$y-$x.txt -t &&
        echo "==================="
    done
done