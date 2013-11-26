#!/usr/bin/python

#. John Pazarzis
# Sun Nov 24 07:14:28 EST 2013

''' 
Reformats a tick file so it can be inserted to mysql
'''

import csv
from os.path import expanduser

currency_pair = 'EUR_USD'

home = expanduser("~")

candles_filename = '{0}/projects/forex/historical-ticks/{1}.csv'.format(home,currency_pair)

def format_timestamp(ts):
    tokens = ts.split(' ')
    day, month, year = tokens[0].split('/')
    year = int(year)+2000
    return '{0}-{1}-{2},{3}'.format(str(year),month,day, tokens[1].replace(':',','))
    
def compare_timestamps(ts1, ts2):
    tokens = ts1.split(' ')
    day, month, year = tokens[0].split('/')
    ts1 = year + month + day + tokens[1]

    tokens = ts2.split(' ')
    day, month, year = tokens[0].split('/')
    ts2 = year + month + day + tokens[1]

    return ts1 > ts2

skipped = 0

def read_ticks():
    time_stamp = ''
    index = 0
    global skipped
       
    for tokens in csv.reader(open(candles_filename,'r')):
        if len(time_stamp) and compare_timestamps(time_stamp, tokens[0]):
            print 'skipping:', time_stamp, tokens[0]
            skipped += 1
            continue
        if len(time_stamp) == 0 or time_stamp != tokens[0]:
            index = 0
            time_stamp = tokens[0]
        else:
            index += 1
        yield format_timestamp(tokens[0]), str(index), tokens[1],tokens[2]

if __name__ == '__main__':
    f = open('EUR_USD_TRANSFORMED.csv','w')
    for i, tick in enumerate(read_ticks()):
        f.write(','.join(tick))
        f.write('\n')
        if i % 1000000 == 0:
            print i

    print 'skipped:',skipped


