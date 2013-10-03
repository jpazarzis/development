#!/usr/bin/python

import random

a,b,c = 10.2, 8.2, 21.31

def create():

        print "SampleData sd[100];"

        x = 0.0

        for i in range(100):
            x += 0.1
            y = a* x *x + b *x + c

            print "sd[{0}].x = {1};".format(i,x),
            print "sd[{0}].y = {1};".format(i,y),
            
            if i % 2 == 1 and i >0:
                    print


def calc_fitness(a1,b1,c1):
    f = 0
    x = 0.0

    for i in range(10):
            x += 0.1
            y =(a* x*x) + b *x + c
            y1 = (a1* x *x) + (b1 *x) + c1
            print y,y1,x,a1,b1,c1
            f += abs(y1-y)

    return f



if __name__ == '__main__':
   create()
    

