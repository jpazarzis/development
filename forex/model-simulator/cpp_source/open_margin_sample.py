#!/usr/bin/python

############################################################################
#
# Author: John Pazarzis
# Date:  Tue Nov  5 08:12:20 EST 2013
# 
# Samples the behavior of open margin
#
############################################################################

leverage = 50.0
lots_per_position = 1
standard_contract = 100000.0


class Position:
    def __init__(self, buy_price):
        self.buy_price = buy_price
        self.margin = 0.0

class Account:
    def __init__(self):
        self.balance = 10000
        self.positions = []
        self.margin = 0

    def equity(self, price):
        eqt = self.balance
        for position in self.positions:
            eqt += (price - position.buy_price) * standard_contract * lots_per_position
        return eqt

    def free_margin(self, price):
        return self.equity(price) - self.margin

    def open_position(self, price):
        self.margin += standard_contract * lots_per_position * price / leverage
        self.positions.append(Position(price))




if __name__ == '__main__':

    prices = [1.341,1.342,1.339,1.351,1.355,1.36, 1.38, 1.39]

    account = Account()

    print '{0:>12} {1:>8} {2:>8} {3:>8}'.format('balance', 'price', 'equity', 'free_margin')
    for i, price in enumerate(prices):
        if i in [0,5,7]:
            account.open_position(price)
        print '{0:>12} {1:>8.4f} {2:>8.0f} {3:>8.0f}'.format(account.balance, price, account.equity(price), account.free_margin(price))
            
        

