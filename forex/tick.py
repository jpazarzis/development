#!/usr/bin/python
import urllib2
import json
import sys

'''
Author: John Pazarzis
Date: Sep. 20 2013

Tick is a class to retrieve real time pricing data from oanda web site
'''

base_url = 'http://api-sandbox.oanda.com/v1/quote?instruments='

all_instruments_url = 'http://api-sandbox.oanda.com/v1/instruments'

class Tick:
    def __init__(self, instruments):
        self.url = '{0}{1}'.format(base_url,'%2C'.join(instruments))

    def retrieve(self):
        response = urllib2.urlopen(self.url)
        json_strg = ''.join(response)

        for values in json.loads(json_strg)["prices"]:
            instrument = values["instrument"]
            time = values["time"]
            bid = values["bid"]
            ask = values["ask"]
            print '{0} {1} {2} {3}'.format(instrument,time, bid, ask)


def show_all_instruments():
    response = urllib2.urlopen(all_instruments_url)
    json_strg = ''.join(response)
    for values in json.loads(json_strg)["instruments"]:
            instrument = values["instrument"]
            print '{0}'.format(instrument)




if __name__ == '__main__':

        show_all_instruments()
        sys.exit(0)

        tick = Tick(['EUR_USD','USD_JPY','NAS100_USD'])
        
        while True:
            tick.retrieve()
            



