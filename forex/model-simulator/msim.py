#!/usr/bin/python
from models import *
from tick_feed_manager import get_feed

if __name__ == '__main__':
    feed = get_feed('EUR_USD')
    m = SpecificMinuteModel()
    feed.register(m)
    m.feed = feed
    feed.start(4000000)
    print m.number_of_buy,m.number_of_sell
    print 'total orders: ', len(m.orders)

    sell_orders = [ order for order in m.orders if order.side == 'SELL']

    print 'sell orders: ', len(sell_orders)
    print 'won: ' , len( [ order for order in sell_orders if order.status == 'WON'])
    print 'lost: ' , len( [ order for order in sell_orders if order.status == 'LOST'])

    print 'value: ' , sum( [ order.value for order in sell_orders])
    print '========'
    buy_orders = [ order for order in m.orders if order.side == 'BUY']

    print 'buy orders: ', len(buy_orders)
    print 'won: ' , len( [ order for order in buy_orders if order.status == 'WON'])
    print 'lost: ' , len( [ order for order in buy_orders if order.status == 'LOST'])

    print 'value: ' , sum( [ order.value for order in buy_orders])

