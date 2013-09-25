#!/usr/bin/python
import datetime 

'''
Implements a Candle Feed Simulator.

Models can subscribe to the simulator by calling its 
subscribe function and providing a call back function
taking a candle as its only parameter. The simulator
will call back for every candle.

'''


history_candles_dir = '../historical-candles'

def make_time(timestamp):
    tokens = timestamp.split(' ')
    t = tokens[0].split('-')
    year, month,  day,   = int(t[0]), int(t[1]), int(t[2])
    t = tokens[1].split(':')
    hour, minute, second = int(t[0]), int(t[1]), int(t[2])
    return datetime.datetime(year, month, day,hour, minute, second)


class Candle:
    def __init__(self,line):
        tokens = line.split(',')
        self.timestamp = make_time(tokens[0])
        self.open = float(tokens[1])
        self.high = float(tokens[2])
        self.close = float(tokens[3])    
        self.low = float(tokens[4])
    
    def __repr__(self):
        return '{0} {1} {2} {3} {4}'.format(self.timestamp, self.open, self.high, self.close, self.low)

class CandleFeedSimulator:
    def __init__(self, instrument):
        self.instrument = instrument
        self.callbacks = []

    def subscribe(self, callback):
        self.callbacks.append(callback)

    def start(self):
        filename = '{0}/{1}_candles.csv'.format(history_candles_dir,self.instrument)
        print filename
        for i, line in enumerate(open(filename)):
            if i == 0 : continue
            candle = Candle(line)
            for callback in self.callbacks:
                try:
                    callback(candle)
                except Exception as ex: 
                    print ex


if __name__ == '__main__':

    def model(candle):
            t = candle.timestamp
            if t.minute == 40:
                print 'from model (40th minute):',
                print candle

    c = CandleFeedSimulator('EUR_USD')
    c.subscribe(model)
    c.start()


            
