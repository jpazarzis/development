#!/usr/bin/python
from tick_feed import get_tick
import random
import cPickle
from time import gmtime, strftime

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
        
        return_value = 1
        engine.current_trade = None
    elif engine.current_trade and tick.bid < engine.current_trade.buy_price - 0.002:
       
        return_value = -1
        engine.current_trade = None
    else:
        return_value = None

    return return_value



def trivial_model(tick,engine):
    max_bid_so_far = max([t.bid for t in engine.ticks])

    if tick.ask > max_bid_so_far and engine.current_trade is None:
        engine.current_trade = BuyTrade(tick.bid)

    if engine.current_trade and tick.bid > engine.current_trade.buy_price + 0.00002:
        return_value = 1
        engine.current_trade = None
    elif engine.current_trade and tick.bid < engine.current_trade.buy_price - 0.00002:
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
        
        


def test_model():
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
        print ' wins: {0}, losses: {1} total: {2}'.format(wins, loses, i)

def test_model2():


    print 'loading ticks...' 
    ticks = load_ticks('./historical-ticks/EUR_USD.csv')

    print 'start processining at: ', strftime("%Y-%m-%d %H:%M:%S", gmtime())
    print 'processing {0} ticks'.format(len(ticks))
    model = ModelEngine(random_model,1000)

    i = 0    
    wins, loses = 0,0
    for tick in ticks:
        i+=1
        r = model.process(tick,i)
        if r is None:
            continue
        if r == 1: wins += 1
        if r == -1: loses += 1
        

    print ' wins: {0}, losses: {1} total: {2}'.format(wins, loses, i)
    print 'done at: ', strftime("%Y-%m-%d %H:%M:%S", gmtime())

def load_ticks(filename):
    i = 0    
    ticks = []
    for tick in get_tick('./historical-ticks/EUR_USD.csv'):
        i += 1
        ticks.append(tick)
        if i >=2000000:
            break
    return ticks

if __name__ == '__main__':
    print 'sample model\n'
    
    test_model2()
    #ticks = load_ticks('./historical-ticks/EUR_USD.csv')
    #create_pickle_file()
    #model = ModelEngine(trivial_model,1000)
    #open_pickle_file()
    
    #test_timimg()

