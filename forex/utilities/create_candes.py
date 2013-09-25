#!/usr/bin/python

'''
Creates minute based candlesticks based on tick data
'''

import sys
import datetime 

history_ticks_dir = '../historical-ticks'
history_candles_dir = '../historical-candles'

def make_time(timestamp):
    tokens = timestamp.split(' ')
    t = tokens[0].split('/')
    day, month, year = int(t[0]), int(t[1]), int(t[2])
    if year <100:
        year += 2000
    t = tokens[1].split(':')
    hour, minute, second = int(t[0]), int(t[1]), int(t[2])
    return datetime.datetime(year, month, day,hour, minute, second)

def create_candles(instrument):
    print 'creating candles for {0} '.format(instrument)
    input_filename = '{0}/{1}.csv'.format(history_ticks_dir,instrument)
    output_filename = '{0}/{1}_candles.csv'.format(history_candles_dir,instrument)
    input_file = open(input_filename)
    output_file = open(output_filename, 'w')
    open_time = None
    output_file.write('timestamp,open,high,close,low\n')

    for i, line in enumerate(input_file):
        if i % 1000000 == 0:
            print '{0} : current line index: {1}'.format(instrument,i)
        tokens = line.rstrip().split(',')
        timestamp, bid,ask = make_time(tokens[0]),float(tokens[1]), float(tokens[2])

        if open_time is None:
            open_time = datetime.datetime(timestamp.year, timestamp.month, timestamp.day, timestamp.hour, timestamp.minute, 0)
            end_time = datetime.datetime(timestamp.year, timestamp.month, timestamp.day, timestamp.hour, timestamp.minute, 59)
            ticks = []            

        if timestamp > end_time:
            if len(ticks) > 0:
                output_file.write('{0},{1},{2},{3},{4}\n'.format(open_time,ticks[0], max(ticks),ticks[-1], min(ticks)))
            open_time = datetime.datetime(timestamp.year, timestamp.month, timestamp.day, timestamp.hour, timestamp.minute, 0)
            end_time = datetime.datetime(timestamp.year, timestamp.month, timestamp.day, timestamp.hour, timestamp.minute, 59)
            ticks = []            
        ticks.append(bid)
    
    print input_filename
    print output_filename

if __name__ == '__main__':
    #create_candles(instrument)
    create_candles('GBP_USD')
    create_candles('USD_CAD')   
    create_candles('USD_CHF')   
    create_candles('USD_JPY')   
    
