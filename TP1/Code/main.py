from errno import ESTALE
import random
import math
import sys
import time
import csv
import argparse
from brute_force import execute_brute_force
from DpR import execute_DpR   
from utils import GRID_SIZE

'''
--------------------------------------------------------------------
ATTENTION : Dans votre code vous devez utiliser le générateur gen.py
pour générer des points. Vous devez donc modifier ce code pour importer
les points depuis les fichiers générés.
De plus, vous devez faire en sorte que l'interface du tp.sh soit
compatible avec ce code (par exemple l'utilisation de flag -e, -a, (p et -t)).
--------------------------------------------------------------------
'''

def main():

    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument( "-a","--algo", type=str)
    parser.add_argument("-e","--execPath",type=str)
    parser.add_argument('-p',action="store_const", const=1,required=False)
    parser.add_argument('-t',action="store_const", const=1,required=False)
    args = parser.parse_args()    
    
    points = [] 
    # Read files
    with open(args.execPath, 'r') as f:
        n=int(f.readline())
        for i in range(n):
            line = f.readline().split(" ")
            points.append((int(line[0]),int(line[1])))
    f.close()

    # Convert to csv In case we need it!
    # with open("../Data.csv", 'w') as f:
    #     writer = csv.writer(f)
    #     for point in points:
    #         writer.writerow([point[0], point[1]])
    # f.close()

    # Execute algorithm    
    if args.algo == "brute":
        # Exécuter l'algorithme force brute
        print("Brute")   
        data = execute_brute_force(points)
    elif args.algo == "recursif":
        # Exécuter l'algorithme Diviser pour régner
        print("Recursif")
        sorted_x = sorted(points, key=lambda x:x[0])
        sorted_y = sorted(points, key=lambda x:x[1])
        data = execute_DpR(sorted_x,sorted_y,3)
     
    elif args.algo == "seuil":
        print("Seuil")
        sorted_x = sorted(points, key=lambda x:x[0])
        sorted_y = sorted(points, key=lambda x:x[1])
        data = execute_DpR(sorted_x,sorted_y,15)
    else :
        #print help.
        print("Invalid")

    if(args.p):
        print("Distance: " + str(data[1]))
    if(args.t):
        print("Temps d'execution: " + str(data[0]))
            
main()