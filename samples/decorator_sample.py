#!/usr/bin/python

def decorate(func):
    print 'inside:',func.__name__
    def wrapper(*args):
        print 'executing:',func.__name__
        return func(*args)
    return wrapper

@decorate
def my_func(par):
    print par



#my_func = decorate(my_func)

my_func('hello')




