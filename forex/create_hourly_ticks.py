#!/usr/bin/python

'''
This utility converts the tick based data to open high low close 
in a hourly base. To be used for comparison against similar data 
exported by metatrader
'''

class Tick:
    def __init__(self, line):
        line = line.rstrip()
        tokens = line.split(',')
        t = tokens[0]
        tt = t.split(' ')
        self.day = tt[0]
        t2 = self.day.split('/')
        self.day = t2[0]
        self.month = t2[1]
        self.year = '20{0}'.format(t2[2])
        self.day = '{0}.{1}.{2}'.format(self.year, self.month, self.day)
        self.time = tt[1]
        t2 = self.time.split(':')
        self.hour = t2[0]
        self.minute = t2[1]
        self.second = t2[2]
        self.timestamp = '{0}:00'.format(self.hour)
        self.bid = float(tokens[1])
        self.ask = float(tokens[2])

    def __repr__(self):
        return '{0} {1} {2} {3}'.format(self.day, self.timestamp, self.bid, self.ask)


def get_tick(filename):

    for line in open(filename):
        yield Tick(line)


if __name__ == '__main__':

    current_day = None    
    current_hour = None    
   
    current_timestamp = None    
    open_price = None
    high_price = None
    low_price = None
    close_price = None
    
    
    for i, tick in enumerate(get_tick('./historical-ticks/EUR_USD.csv')):

        new_hour = tick.hour

        if current_hour is None or new_hour != current_hour:
            if current_day is not None:
                print ','.join( [ str(x) for x in [current_day, current_timestamp,open_price, high_price, low_price, close_price] ])

            current_day = tick.day    
            current_hour = tick.hour
            
            current_timestamp = tick.timestamp

            open_price = tick.bid
            high_price = tick.bid
            low_price = tick.bid
            close_price = tick.bid
        else:
            if tick.bid > high_price:
                high_price = tick.bid

            if tick.bid < low_price:
                low_price = tick.bid

            close_price = tick.bid
