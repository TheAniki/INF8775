#!/usr/bin/bash

helpFunction()
{
   echo ""
   echo "Usage: $0 -e [path_vers_exemplaire] -c [nombre_de_circonscriptions] [-p] "
   exit 1 # Exit script after printing help
}

AFFICHER_SOLUTION=""
AFFICHER_TEMPS=""

while getopts "e:c:p" opt
do
   case "$opt" in
      c ) nbCirconscriptions="$OPTARG" ;;
      e ) path_vers_exemplaire="$OPTARG" ;;
      p ) AFFICHER_SOLUTION="-p" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done


# Print helpFunction in case parameters are empty
if [ -z "$nbCirconscriptions" ] || [ -z "$path_vers_exemplaire" ] 
then
   echo "Some or all of the parameters are empty"
   helpFunction
fi

# Begin script in case all parameters are correct
make clean && make && ./TP3 -e $path_vers_exemplaire -c $nbCirconscriptions $AFFICHER_SOLUTION $AFFICHER_TEMPS