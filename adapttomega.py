# a script that transforms a matrix into a lower triangular matrix
# and then prints the matrix
#

import numpy as np
import sys
import math
import random
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.patches as patches
import matplotlib.path as path
import matplotlib.lines as lines
import matplotlib.text as text
import matplotlib.image as image
import matplotlib.figure as figure
import matplotlib.cm as cm
import matplotlib.colors as colors
import matplotlib.colorbar as colorbar
import matplotlib.ticker as ticker
import matplotlib.legend as legend
import matplotlib.markers as markers
import matplotlib.axis as axis


def main():
    # read the matrix from the command line
    print("Reading matrix from command line...")

    with open(sys.argv[1]) as f:
        ncols = len(f.readline().split(' '))

    matrix = np.loadtxt(sys.argv[1], delimiter=' ', usecols=range(1,ncols))  
    names = np.loadtxt(sys.argv[1], delimiter=' ', usecols=0, dtype=str) 
    # create the lower triangular matrix
    print(matrix)
    lower = np.tril(matrix)
    # print the matrix
    # put to 0 diagonal elements
    for i in range(0,lower.shape[0]):
        lower[i,i] = 0
    print(lower)
    

    #print to a file a fragment of formatted text
    with open(sys.argv[2], 'w') as f:
        f.write("#mega\n")
        f.write("!Title: Concatenated Files;" + "\n")
        f.write("!Format DataType=Distance DataFormat=LowerLeft NTaxa="+ str(ncols-1) + ";\n")
        f.write("!Description" + "\n")
        f.write("  No. of Taxa : 20" + "\n")
        f.write("  No. of Groups : " + str(ncols-1) + "\n")
        f.write("  Gaps/Missing data : Pairwise Deletion" + "\n")
        f.write("  Codon Positions : 1st+2nd+3nd+Noncoding" + "\n")
        f.write("  Distance method : Nucleotide: Tamura-Nei  [Pairwise distances]" + "\n")
        f.write("  No. of Sites : 1369989" + "\n")
        f.write("  d : Estimate" + "\n")
        f.write(";\n")
        f.write("\n")


    # print to a file line by line the names of the genes
    print("Printing to file...")
    with open(sys.argv[2], 'a') as f:
        for i in range(0,lower.shape[0]):
            f.write("["+ str(i+1)    +"] #" + names[i] + "\n")
    with open(sys.argv[2], 'a') as f:
        f.write("\n")
        f.write("\n")
        #print row numbers one by one
        f.write("[  ")
        for i in range(0,lower.shape[0]):
            f.write(str(i+1)+"  ")
        f.write("]")
        f.write("\n") 
    #print to the same file the matrix with the number of the row first
    with open(sys.argv[2], 'a') as f:
        for i in range(0,lower.shape[0]):
            f.write("["+ str(i+1)    +"] ")
            for j in range(0,lower.shape[1]):
                if lower[i,j]!=0.0:
                    f.write(str(lower[i,j]) + " ")
            f.write("\n")

main()
