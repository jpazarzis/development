#!/usr/bin/python

# john pazarzis, Nov 17 2012
# in this example we are trying to solve an equation of the following form:
# y = ax1 + bx2 + cx3
# the unknown quantities will be a, b,c while y, x1, x2, x3 will be provided

import random
import ga
import math

class QuadraticEquation:
    a, b, c , length, min_value, max_value  = 12, 31, 15, 6, 0, 100
    x= [ [random.uniform(min_value, max_value) , random.uniform(min_value, max_value), random.uniform(min_value, max_value)] for i in range(length) ]
    y = [a*d[0] + b * d[1] + c * d[2] for d in x]

    @staticmethod
    def get_correct_values():
        return QuadraticEquation.a,QuadraticEquation.b,QuadraticEquation.c

    @staticmethod
    def fitness_calculator(weights):
        a, b, c, diff = weights[0], weights[1], weights[2], 0.0
        x, y = QuadraticEquation.x, QuadraticEquation.y
        for i in range(len(QuadraticEquation.y)):
            v = (a*x[i][0] +b*x[i][1] + c*x[i][2] - y[i])
            v = v ** 2
            diff += v
        diff = math.sqrt(diff)
        return 1.0 / diff

if __name__ == '__main__':
        p = ga.GeneticAlgorithm(QuadraticEquation.fitness_calculator,  3)
        p.run()
        print 'estimated values ...: ', p.winner()
        print 'correct values .....: ', QuadraticEquation.get_correct_values()







