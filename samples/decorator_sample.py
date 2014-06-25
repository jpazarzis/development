#!/usr/bin/python

def add_tag(tag):
    def add_html(f):
        def inner_f(name):
            return tag+name+tag
        return inner_f
    return add_html


@add_tag('html')
def gethello(name):
    return 'hello ' + name



if __name__ == '__main__':
    f1 = gethello
    print f1('john')

    @add_tag('junk')
    def gethello(name):
            return f1(name) 


    print gethello('john')


