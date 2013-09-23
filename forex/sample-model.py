#!/usr/bin/python
from tick_feed import get_tick
import random
'''
This program implements a sample model using the tick feed, simulating the
behavior of a simple strategy. We can create more sophisticate models using
the same approach. What needs to be changed is the process fucntion of the 
Model where we need to descibe our trading logic...
'''


class BuyTrade:
    def __init__(self, buy_price):
        self.buy_price = buy_price

def random_model(tick,engine):
    r = random.random()
    if r >= 0.9 and engine.current_trade is None:
        engine.current_trade = BuyTrade(tick.ask)
        return None

    if engine.current_trade and tick.bid > engine.current_trade.buy_price + 0.002:
        print 'won', 'bought at: ', engine.current_trade.buy_price, 'sold at: ',tick.bid,
        return_value = 1
        engine.current_trade = None
    elif engine.current_trade and tick.bid < engine.current_trade.buy_price - 0.002:
        print 'lost', 'bought at: ', engine.current_trade.buy_price, 'sold at: ',tick.bid,
        return_value = -1
        engine.current_trade = None
    else:
        return_value = None

    return return_value



def trivial_model(tick,engine):
    max_bid_so_far = max([t.bid for t in engine.ticks])

    if tick.ask > max_bid_so_far and engine.current_trade is None:
        engine.current_trade = BuyTrade(tick.bid)

    if engine.current_trade and tick.bid >= engine.current_trade.buy_price + 0.00002:
        return_value = 1
        engine.current_trade = None
    elif engine.current_trade and tick.bid <= engine.current_trade.buy_price - 0.00002:
        return_value = -1
        engine.current_trade = None
    else:
        return_value = None

    return return_value

    

class ModelEngine:
    def __init__(self, model, window_size = 200):
        self.window_size = window_size
        self.ticks = [] # the tick window
        self.current_trade = None
        self.model = model

    def process(self, tick,index):
        if len(self.ticks) < self.window_size:
            # we do not have enough ticks yet
            self.ticks.append(tick)
            return None

        return_value = self.model(tick,self)
        self.ticks.pop(0)
        self.ticks.append(tick)
        return return_value
        
        
        

if __name__ == '__main__':
    #model = ModelEngine(trivial_model,1000)
    model = ModelEngine(random_model,1000)

    i = 0    
    wins, loses = 0,0
    for tick in get_tick('./historical-ticks/EUR_USD.csv'):
        i+=1
        r = model.process(tick,i)
        if r is None:
            continue
        if r == 1: wins += 1
        if r == -1: loses += 1
        print ' wins: {0}, losses: {1}'.format(wins, loses)

    

