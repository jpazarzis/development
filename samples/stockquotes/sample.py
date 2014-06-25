#!/usr/bin/env python

import json
import pprint
import httplib2

def get_stock_content(symbol):
    url = 'http://finance.google.com/finance/info?q=%s' % symbol
    resp, content = httplib2.Http().request(url)
    return content

def get_price(doc):
    doc = doc.replace('//', '')
    return json.loads(doc)[0]['l_cur']

def quote_retriever(symbol):   
    return get_price(get_stock_content(symbol))   

class Quote:
    def __init__(self, symbol):
        self.symbol = symbol
        self.retriever = quote_retriever

    def __str__(self):
        return '{0} : {1}'.format(self.symbol, self.quote)

    @property
    def quote(self):
        return self.retriever(self.symbol)
        




if __name__ == '__main__':
    symbols = ['IBM', 'MSFT']
    quotes = [ Quote(symbol) for symbol in  symbols]
    
    for q in quotes: print q


    

    #print 'ticker: %s' % quote['t']
    #print 'current price: %s' % quote['l_cur']
    #print 'last trade: %s' % quote['lt']
    #print 'full quote:'
    #pprint.pprint(quote)

