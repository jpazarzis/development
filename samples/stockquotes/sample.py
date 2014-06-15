#!/usr/bin/env python

import json
import pprint
import httplib2

def quote_retriever(ticker_symbol):   
    url = 'http://finance.google.com/finance/info?q=%s' % ticker_symbol
    resp, content = httplib2.Http().request(url)   
    content = content.replace('//', '')
    return json.loads(content)[0]['l_cur']
    


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

