#!/usr/bin/python
from tick_feed import get_tick

'''
This program implements a sample model using the tick feed, simulating the
behavior of a simple strategy. We can create more sophisticate models using
the same approach. What needs to be changed is the process fucntion of the 
Model where we need to descibe our trading logic...
'''


class BuyTrade:
    def __init__(self, buy_price):
        self.buy_price = buy_price
    

class Model:
    def __init__(self, window_size = 200):
        self.window_size = window_size
        self.ticks = [] # the tick window
        self.current_trade = None

    def process(self, tick,index):
        
        return_value = None
        
        # if the tick window is complete let's see if this tick
        # is creating a signal
        if len(self.ticks) == self.window_size:
            max_bid_so_far = max([t.bid for t in self.ticks])

            if tick.bid > max_bid_so_far and self.current_trade is None:
                self.current_trade = BuyTrade(tick.bid)

            if self.current_trade and tick.bid >= self.current_trade.buy_price + 0.00002:
                return_value = 1
                self.current_trade = None
            elif self.current_trade and tick.bid <= self.current_trade.buy_price - 0.0001:
                return_value = -1
                self.current_trade = None

        # if the tick window is full, let's remove the first tick
        # so we can add the new one in the list    
        if len(self.ticks) == self.window_size:
            self.ticks.pop(0)

        self.ticks.append(tick)
        return return_value 
        
        
        

if __name__ == '__main__':
    model = Model(1000)
    i = 0    
    wins, loses = 0,0
    for tick in get_tick('./historical-ticks/EUR_USD.csv'):
        i+=1
        r = model.process(tick,i)
        if r is None:
            continue
        if r == 1: wins += 1
        if r == -1: loses += 1
        print wins, loses

    

