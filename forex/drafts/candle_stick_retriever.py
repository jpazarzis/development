#!/usr/bin/python

import urllib2
import json
import sys
from tick import Tick
from tick import show_all_instruments
import time

url= 'http://api-sandbox.oanda.com/v1/history?instrument=EUR_USD&count=5000&candleFormat=midpoint'

def retrieve():
        response = urllib2.urlopen(url)
        json_strg = ''.join(response)

        v = json.loads(json_strg)
        
        instrument = v["instrument"]
        granularity = v["granularity"]
        for c in v["candles"]:
                #print c[time], c.openMid, c.highMid, c.lowMid, c.closeMid, c.volume
                t = c["time"]
                day = t[0:10]
                day = int(day.replace('-',''))
                time = t[11:-1]
                print instrument, granularity, day, time, c["openMid"], c["highMid"], c["lowMid"], c["closeMid"], c["volume"]



if __name__ == '__main__':
    retrieve()        
        



