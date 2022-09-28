#!/bin/bash

helpFunction()
{
   echo ""
   echo "Usage: $0 -a [brute | recursif | seuil] -e CHEMIN_EXEMPLAIRE [-p] [-t]"
   exit 1 # Exit script after printing help
}

AFFICHER_SOLUTION=""
AFFICHER_TEMPS=""

while getopts "a:e:pt" opt
do
   case "$opt" in
      a ) algo="$OPTARG" ;;
      e ) CHEMIN_EXEMPLAIRE="$OPTARG" ;;
      p ) AFFICHER_SOLUTION="-p" ;;
      t ) AFFICHER_TEMPS="-t" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done


# Print helpFunction in case parameters are empty
if [ -z "$algo" ] || [ -z "$CHEMIN_EXEMPLAIRE" ] 
then
   echo "Some or all of the parameters are empty"
   helpFunction
fi

# Begin script in case all parameters are correct
python3 ./Code/main.py -a $algo -e $CHEMIN_EXEMPLAIRE $AFFICHER_SOLUTION $AFFICHER_TEMPS