#!/usr/bin/python
import datetime 

standard_contract = {
'EUR_USD' : 100000.0,
'GBP_USD' : 100000.0,
'USD_CAD' : 100000.0,
'USD_CHF' : 150000.0,
'USD_JPY' : 15000000.0
}

tick_feeds = {}

history_ticks_dir = '../historical-ticks'



def make_time(timestamp):
    tokens = timestamp.split(' ')
    t = tokens[0].split('/')
    day, month, year = int(t[0]), int(t[1]), int(t[2])
    if year <100:
        year += 2000
    t = tokens[1].split(':')
    hour, minute, second = int(t[0]), int(t[1]), int(t[2])
    return datetime.datetime(year, month, day,hour, minute, second)

class Tick:
    def __init__(self, instrument, timestamp, bid, ask):
        self.instrument = instrument
        self.timestamp = timestamp
        self.bid = bid
        self.ask = ask

class TickFeed:
    def __init__(self, instrument):
        self.instrument = instrument
        self.listeners = []
    
    def start(self, max_ticks = None):
        filename = '{0}/{1}.csv'.format(history_ticks_dir,self.instrument) 
        
        for i, line in enumerate(open(filename)):
            if max_ticks and i >= max_ticks:
                break
            if i % 10000 == 0:
                print i
            tokens = line.rstrip().split(',')
            timestamp, bid,ask = make_time(tokens[0]),float(tokens[1]), float(tokens[2])
            tick = Tick(self.instrument, timestamp, bid, ask)
            for listener in self.listeners:
                listener(tick)

    def register(self, listener):
        self.listeners.append(listener)

    def unregister(self, listener):
        self.listeners.remove(listener)

        
    

def get_feed(instrument):
    global tick_feeds

    if instrument not in standard_contract:
        raise Expection('instrument is not supported') 

    if instrument not in tick_feeds:
        tick_feeds[instrument] = TickFeed(instrument)
        
    return tick_feeds[instrument]
    

if __name__ == '__main__':
    def test_model(tick):
        print 'test_model', tick.timestamp, tick.instrument,tick.bid, tick.ask        

    def test_model2(tick):
        print 'test_model2', tick.bid

    # sample use:
    feed = get_feed('EUR_USD')
    feed.register(test_model)
    feed.register(test_model2)
    feed.start()


    






 
