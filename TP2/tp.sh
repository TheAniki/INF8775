#!/bin/bash

helpFunction()
{
   echo ""
   echo "Usage: $0 -a [glouton | progdyn | tabou] -e [path_vers_exemplaire] [-p] [-t]"
   exit 1 # Exit script after printing help
}

AFFICHER_SOLUTION=""
AFFICHER_TEMPS=""

while getopts "a:e:c:pt" opt
do
   case "$opt" in
      a ) algo="$OPTARG" ;;
      e ) path_vers_exemplaire="$OPTARG" ;;
      p ) AFFICHER_SOLUTION="-p" ;;
      t ) AFFICHER_TEMPS="-t" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done


# Print helpFunction in case parameters are empty
if [ -z "$algo" ] || [ -z "$path_vers_exemplaire" ] 
then
   echo "Some or all of the parameters are empty"
   helpFunction
fi

# Begin script in case all parameters are correct
make clean && make && ./TP2 -a $algo -e $path_vers_exemplaire $AFFICHER_SOLUTION $AFFICHER_TEMPS