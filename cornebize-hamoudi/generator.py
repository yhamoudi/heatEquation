#!/usr/bin/env python3

import sys
import random

if __name__ == "__main__":
    """
        Generate randomly a file following the specification.
        Width, height, p and t must be given as argument (in this order).
    """
    if len(sys.argv) != 5:
        print("Syntax: {0} <width> <height> <p> <t>".format(sys.argv[0]))
        sys.exit()
    random.seed()
    width = int(sys.argv[1])
    height = int(sys.argv[2])
    p = float(sys.argv[3])
    t = int(sys.argv[4])
    print(width)
    print(height)
    print(p)
    print(t)
    nb_input = random.randint(0,width*height)
    nb_request = random.randint(0,width*height)
    for z in range(0,nb_input):
        i = random.randint(0,height-1)
        j = random.randint(0,width-1)
        x = random.uniform(0,1)
        print("0 {0} {1} {2}".format(str(i),str(j),str(x)))
    for z in range(0,nb_request):
        i = random.randint(0,height-1)
        j = random.randint(0,width-1)
        print("2 {0} {1} 0".format(str(i),str(j)))
