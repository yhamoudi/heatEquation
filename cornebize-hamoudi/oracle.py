#!/usr/bin/env python3

import copy

def compute(grid,p,width,height):
    gridCopy = copy.deepcopy(grid)
    for i in range(0,height):
        for j in range(0,width):
            grid[i][j] = (1-p)*gridCopy[i][j] + \
                p*(gridCopy[i-1][j]+gridCopy[(i+1)%height][j]+gridCopy[i][j-1]+gridCopy[i][(j+1)%width])/4

if __name__ == "__main__":
    width = int(input(""))
    height = int(input(""))
    p = float(input(""))
    t = int(input(""))
    grid = [[0]*width for i in range(0,height)]
    isComputed = False
    while True:
        try:
            s = input("").split(" ")
            mode = int(s[0])
            i = int(s[1])
            j = int(s[2])
            content = float(s[3])
        except EOFError:
            break
        if mode == 0:
            grid[i][j] = content
        if mode == 2:
            if not isComputed:
                for iteration in range(0,t):
                    compute(grid,p,width,height)
                isComputed = True
            print(grid[i][j])
