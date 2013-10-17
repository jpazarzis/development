#!/usr/bin/python
import urllib2
import json

'''
Author: John Pazarzis
Date: Sep. 20 2013

Tick is a class to retrieve real time pricing data from oanda web site
'''

base_url = 'http://api-sandbox.oanda.com/v1/quote?instruments='

class Tick:
    def __init__(self, instrument):
        self.instrument = instrument
        self.time = None 
        self.bid = None
        self.ask = None

    def retrieve(self):
        url = '{0}{1}'.format(base_url,self.instrument)
        response = urllib2.urlopen(url)
        self._populate( ''.join(response))

    def _populate(self, json_strg):
        values = json.loads(json_strg)["prices"][0]
        self.instrument = values["instrument"]
        self.time = values["time"]
        self.bid = values["bid"]
        self.ask = values["ask"]

    def __repr__(self):
        return '{0} {1} {2} {3}'.format(self.instrument,self.time, self.bid, self.ask)

if __name__ == '__main__':
        tick = Tick('EUR_USD')
        print tick
        while True:
            tick.retrieve()
            print tick



