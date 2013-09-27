#!/usr/bin/python

import scipy.stats
import numpy

class Enum(set):
    def __getattr__(self, name):
        if name in self:
            return name
        raise AttributeError


ORDER_SIDE = Enum(('BUY', 'SELL'))

ORDER_STATUS = Enum(('WON', 'LOST', 'OPEN'))


class Order:
    def __init__(self, side,tick, stop_loss, take_profit):
        self.side = side
        self.stop_loss = stop_loss
        self.take_profit = take_profit
        self.value = 0.0

        if self.side == ORDER_SIDE.BUY:
            self.cost = tick.ask
        elif self.side == ORDER_SIDE.SELL:
            self.cost = tick.bid

        self.status = ORDER_STATUS.OPEN
        self.feed = None
        
    def __call__(self,tick):
        if not self.status == ORDER_STATUS.OPEN:
            return
        if self.side == ORDER_SIDE.BUY:
                current_price = tick.bid
                delta = abs(current_price - self.cost)
                if current_price <= self.stop_loss:
                    self.value = (-1.0) * delta * 100000
                    self.status = ORDER_STATUS.LOST
                elif current_price >= self.take_profit:
                    self.value = delta * 100000
                    self.status = ORDER_STATUS.WON
                    
        elif self.side == ORDER_SIDE.SELL:
                current_price = tick.ask
                delta = abs(current_price - self.cost)
                if current_price >= self.stop_loss:
                    self.value = (-1.0) * delta * 100000
                    self.status = ORDER_STATUS.LOST
                elif current_price <= self.take_profit:
                    self.value = delta * 100000
                    self.status = ORDER_STATUS.WON

        if not self.status == ORDER_STATUS.OPEN:
            self.feed.unregister(self)
            


class VanilaModel:
    def __init__(self):
        self.PNL = 0
    
    def __call__(self,tick):
        print 'test_model', tick.timestamp, tick.instrument,tick.bid, tick.ask        

class SpecificMinuteModel:
    '''
    This model trades once every hour, always in a specific minute.

    Is considering Open - Close prices from the beginning of the hour
    to the specific minute to make its decision.

    trading_minute: the minute to trade

    p1:  bid price for the first tick of the hour
    p2:  bid price for the last tick of the trading_minute

    when p1 < p2 then the trade will be SELL if:

    abs(p1 - p2) > SELLING_SPREAD

    using sell_stop_loss and sell_take_profit prices

    when p1 > p2 then the trade will be BUY if:

    abs(p1 - p2) > BUY_SPREAD

    using buy_stop_loss and buy_take_profit prices

    The objective of the simulator will be to optimize these variables
    '''
    def __init__(self, bid_move1 = 0.001, bid_move2 = 0.001, delta_move = 1.0):
        self.PNL = 0
        self.current_hour = 9999
        self.trading_minute = 40
        self.has_triggered = False
        self.deltas = []
        self.mean = -0.271714467814 
        self.std = 14.0694776224
        self.number_of_buy = 0 
        self.number_of_sell = 0
        self.feed = None
        self.orders = []

        self.bid_move1 = bid_move1 
        self.bid_move2 = bid_move2
        self.delta_move = delta_move



    def __call__(self,tick):
        if tick.timestamp.hour != self.current_hour:
            self.current_hour = tick.timestamp.hour
            self.open_price = tick.bid
            self.has_triggered = False
        elif tick.timestamp.minute == self.trading_minute and not self.has_triggered:
            closing_price = tick.bid
            delta = self.open_price - closing_price
            delta *= 10000
            self.deltas.append(delta)
            order = None
            if delta >= self.mean + self.delta_move * self.std:
                self.number_of_sell += 1    
                order = Order(ORDER_SIDE.SELL, tick, tick.bid + self.bid_move1, tick.bid - self.bid_move2)
            #elif delta <= self.mean - 1.0 * self.std:
            #    self.number_of_buy += 1    
            #    order = Order(ORDER_SIDE.BUY, tick, tick.ask - 0.001, tick.ask + 0.001)

            if order:
                order.feed = self.feed
                self.orders.append(order)
                self.feed.register(order)

            self.has_triggered = True

    def __repr__(self):
        s = '{0},{1},{2},'.format(self.bid_move1, self.bid_move2, self.delta_move)
        sell_orders = [ order for order in self.orders if order.side == 'SELL']
        s += '{0},'.format(len(sell_orders))
        s += '{0},'.format(len( [ order for order in sell_orders if order.status == 'WON']))
        s += '{0},'.format(len( [ order for order in sell_orders if order.status == 'LOST']))
        s += '{0}'.format(sum( [ order.value for order in sell_orders]))
        return s
    


if __name__ == '__main__':
    m = VanilaModel()
    m('test')
