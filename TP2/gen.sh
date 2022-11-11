
for y in 100 1000 10000; do
    echo $y: &&
    for x in 10 100 1000; do
        for z in {1..9}; do
            echo "./exemplaires/WC-"$y"-"$x"-0"$z".txt" &&
            ./TP2 -a glouton -e ./exemplaires/WC-$y-$x-0$z.txt -t &&
            echo "==================="
        done
        echo "./exemplaires/WC-"$y"-"$x"-10.txt" &&
        ./TP2 -a glouton -e ./exemplaires/WC-$y-$x-10.txt -t &&
        echo "==================="
    done
done