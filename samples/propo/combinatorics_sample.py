#!/usr/bin/python

from math import factorial
from itertools import combinations
from itertools import product

import cProfile

class Combo:
    def __init__(self, combos):
        self.combos = 0
        for i in combos:
            self.combos = self.combos | (1 << (i-1))
            
    def update_matches(self, combosList):
        self.matches = [ x for x in combosList if self in x ]

    def __contains__(self, other):
        return other.combos & self.combos == other.combos

    
def count_combos(N,K,L):
    a = range(1,N+1)
    combos3 = [Combo(c) for c in  combinations(a,K)]
    combos2 = [Combo(c) for c in  combinations(a,L)]
    final_combos = []
    while len(combos3) > 0:
        map( lambda c2: c2.update_matches(combos3), combos2)    

        combos2.sort(key=lambda x: len(x.matches), reverse = True)
        if len(combos2) > 0 :
            combos2 = [ c for c in combos2 if len(c.matches) >0 ]
        if len(combos2) == 0:
            break

        n = len(combos2[0].matches)
        top_combos = []
        for cc in combos2:
            if len(cc.matches) != n:
                break
            top_combos.append(cc)
        top_combos.sort(key = lambda y: y.combos)

        map(lambda x:combos3.remove(x), top_combos[0].matches)

        final_combos.append(top_combos[0])
        combos2.remove(top_combos[0])
    print len(final_combos)



if __name__=='__main__':
    #count_combos(16,3,2)
    #count_combos(18,3,2)
    #count_combos(12,5,4)

    cProfile.run('count_combos(10,5,4)')




