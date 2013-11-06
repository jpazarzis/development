#!/usr/bin/python
'''
Author: john pazarzis
Date: Wed Nov  6 10:12:17 EST 2013

Based in a file containing the running balance applies a fitness function
calculating it.
'''

values = None

def get_max_drawdown():
    ''' given a list of values returns the max drawdown '''
    if len(values) == 0:
        return 0;
    maxdrawdown, max_so_far = 0.0, values[0]
    for v in values:
        if v > max_so_far:
            max_so_far = v * 1.0
        else:
            if max_so_far > 0:
                dd = (v - max_so_far) / max_so_far
                if dd < maxdrawdown:
                    maxdrawdown = dd
    return abs(dd)
    

def brutal_fitness_function():
    ''' Most brutal fitness function which only relies in final balance'''
    return values[-1]

def final_balance_over_maxdd():
    ''' Fitness equals final balance divided by max draw down '''
    return values[-1] / get_max_drawdown()

# You can add any fitness function you need using a similar implementation to
# brutal_fitness_function. Note that any function implemented in this module has
# automatic access to the running balance which is a global list that is firstly
# loaded upon execution of the program....  More than this you need to add the
# name of the new function to the ff list so it will be processed when the
# program will be executed....

# The following is a sample output:
#
# Testing the behavior of fitness functions
#
# brutal_fitness_function 54427.0
# final_balance_over_maxdd 17277091.7035



if __name__ == '__main__':
    ff = [brutal_fitness_function, final_balance_over_maxdd] 
    values = [ float(linestrg) for linestrg in open("running_balance.csv") ] 

    print
    print 'Testing the behavior of fitness functions'
    print

    for f in ff:
        print f.__name__, f()
    

