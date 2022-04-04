#!/usr/bin/python3

import os
import sys

def chooseVesion(choice):
    if choice == "s":
        return "-L./serial -lserial_bblas"
    elif choice == "p":
        return "-L./pthreads -lpthreads_bblas"
    elif choice == "o":
        return "-L./openmp -lopenmp_bblas"


def chooseAlgo(choice):
    if choice == "dot":
        return "-DDOT"
    elif choice == "vvm":
        return "-DVVM"
    elif choice == "mvv":
        return "-DMVV"
    elif choice == "mmm":
        return "-DMMM"


def chooseFlags(choice):
    if choice == "0":
        return "-O0"
    elif choice == "1":
        return "-O1"
    elif choice == "2":
        return "-O2"
    elif choice == "3":
        return "-O3"
    elif choice == "f":
        return "-Ofast"

main_directory = os.getcwd()

line = ""

while open("test.inc", "r+") as f:
    line = f.readline()
    print(line)

f.close()