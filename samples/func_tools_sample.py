#!/usr/bin/python

import functools

def foo(x,y):
    print x, y

foo=functools.partial(foo,y=7)
foo(8)






