#!/usr/bin/python

import scipy.stats
import numpy

class VanilaModel:
    def __init__(self):
        self.PNL = 0
    
    def __call__(self,tick):
        print 'test_model', tick.timestamp, tick.instrument,tick.bid, tick.ask        

class SpecificMinuteModel:
    '''
    This model trades once every hour, always in a specific minute.

    Is considering Open - Close prices from the beginning of the hour
    to the specific minute to make its decision.

    trading_minute: the minute to trade

    p1:  bid price for the first tick of the hour
    p2:  bid price for the last tick of the trading_minute

    when p1 < p2 then the trade will be SELL if:

    abs(p1 - p2) > SELLING_SPREAD

    using sell_stop_loss and sell_take_profit prices

    when p1 > p2 then the trade will be BUY if:

    abs(p1 - p2) > BUY_SPREAD

    using buy_stop_loss and buy_take_profit prices

    The objective of the simulator will be to optimize these variables
    '''
    def __init__(self):
        self.PNL = 0
        self.current_hour = 9999
        self.trading_minute = 40
        self.has_triggered = False
        self.deltas = []

    def __call__(self,tick):
        if tick.timestamp.hour != self.current_hour:
            self.current_hour = tick.timestamp.hour
            self.open_price = tick.bid
            self.has_triggered = False
        elif tick.timestamp.minute == self.trading_minute and not self.has_triggered:
            closing_price = tick.bid
            delta = self.open_price - closing_price
            self.deltas.append(delta*10000)
            self.has_triggered = True
        
    def show_stats(self):
        deltas = self.deltas
        print 'count', len(deltas)
        print 'min:{0} max:{1}'.format(min(deltas),max(deltas))
        mean = numpy.average(deltas)   
        std = numpy.std(deltas)
        p = scipy.stats.normaltest(deltas)[1]
        print 'mean: {0} std: {1}'.format(mean, std)
        print 'p = ',p,
        if p >= 0.5:
            print 'Is Normal'
        else:
            print 'not normal'
        


if __name__ == '__main__':
    m = VanilaModel()
    m('test')
