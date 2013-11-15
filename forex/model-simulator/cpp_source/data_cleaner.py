#!/usr/bin/python

'''
* Author: John Pazarzis
* Fri Nov 15 10:31:11 EST 2013
*
* Receives a file containing data to be used for neural network training
* removing conflicting data points
* 
* The format of the file should be the one expected by FANN 
* The output of the program will be a FANN compatible file...
'''
import itertools
import math
import sys

class Data:
    '''
    Holds input - output data as they are described in the FANN based file
    '''
    def __init__(self, line1, line2):
        self.input = [ float(s) for s in  line1.split(' ') ]
        self.output = [ float(s) for s in  line2.split(' ') ]

    def __repr__(self):
        return str(self.output)

class Distance:
    '''
    calculates and holds the Euclidean distance of two data points
    '''
    def __init__(self, data1, data2):
        self.data1 = data1
        self.data2 = data2
        d1,d2 = data1.input, data2.input
        assert(len(d1) == len(d2))
        self.distance = math.sqrt(sum([ (pair[0] - pair[1]) **2 for pair in zip(d1,d2)]))

    def __repr__(self):
        return '{0:6.2f} {1} {2}'.format(self.distance, self.data1,self.data2)

def load_data(filename):
    '''
    loads the FANN data to a list that will be cleaned up
    '''
    d=[]
    lines = [line for line in open(filename)]
    c = 1 #skip first line (contains structure of data)
    while c < len(lines):
        line1 = lines[c].strip()
        if len(line1) == 0:
            c+=1
            continue # allow for a blank line between input / output
        c += 1
        line2 = lines[c].strip()
        d.append(Data(line1,line2))
        c += 1
    return d

def make_distances(data):
    '''
    receives a list of data returning all possible pairs with their distances
    '''
    distances = []
    for c in itertools.combinations(data,2):
        distances.append(Distance(c[0],c[1]))
    distances.sort(key = lambda d: d.distance)
    return distances

def clean_data(data, distances, cleanup_rate = 0.8):
    '''
    cleans up the data removing the top conflicting pairs based in their
    Euclidean distance
    '''
    cutoff_index = len(data) * cleanup_rate
    for d in distances:
        if len(data) <= cutoff_index:
            break
        if d.data1.output == d.data2.output:
            continue
        if d.data1 in data:
            data.remove(d.data1)
        if d.data2 in data:
            data.remove(d.data2)

def create_clean_file(data,filename):
    f = open(filename, 'w')
    f.write('{0} {1} {2}\n'.format(len(data), len(data[0].input),len(data[0].output)))
    for d in data:
        for v in d.input:
            f.write('{0} '.format(v))
        f.write('\n')
        for v in d.output:
            f.write('{0} '.format(v))
        f.write('\n')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'please provide the name of the file in the command line'
        sys.exit(0)
    filename = sys.argv[1]
    data = load_data(filename)
    distances = make_distances(data)
    clean_data(data, distances)
    create_clean_file(data, filename+'.clean')


