#!/usr/bin/python

import urllib2

response = urllib2.urlopen('http://api-sandbox.oanda.com/v1/quote?instruments=EUR_USD')

line = ''.join(response)
print line

