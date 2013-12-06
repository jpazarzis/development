#!/usr/bin/python
import sys
import csv
import tools.dbtools

sql_select_ask_price = '''select ask from  
                            {0} 
                          where date='{1}' 
                          AND  hour = {2} LIMIT 1'''

sql_select_target_price = '''select bid, date, hour, minute,second from {0} WHERE  date = '{1}' AND  hour >= {2} AND hour < {3}  and bid >= {4} LIMIT 1 '''

sql_select_stop_loss_price = '''select bid, date, hour, minute,second 
                             from {0} 
                            WHERE  date = '{1}' AND  hour >= {2} AND hour < {3}  
                            and bid <= {4} LIMIT 1 '''

sql_exit_price = '''select bid from  {0} where date='{1}' AND  hour = {2} LIMIT 1'''

class Trade:
    def __init__(self, tokens):
        self.date = tokens[0]
        self.hour = int(tokens[1])
        self.expiration_hour = int(tokens[2])
        self.take_profit_pips = int(tokens[3])
        self.stop_loss_pips = 140

    def load_take_profit_point(self):
        global success_count
        
        self.reached_take_profit = False
        sql = sql_select_target_price.format(currency_pair, self.date, self.hour,self.expiration_hour, self.take_profit_price)
        for row in tools.dbtools.execute_query(sql):
            self.profit_bid = row.bid
            self.profit_date = '{0}:{1:02}:{2:02}:{3:02}'.format(row.date,row.hour,row.minute,row.second)
            self.reached_take_profit = True
            success_count += 1
        

    def load_stop_loss_point(self):
        global failure_count
        self.reached_stop_loss = False
        sql = sql_select_stop_loss_price.format(currency_pair, self.date, self.hour,self.expiration_hour, self.take_profit_price)
        for row in tools.dbtools.execute_query(sql):
            self.stop_loss_bid = row.bid
            self.stop_loss_date = '{0}:{1:02}:{2:02}:{3:02}'.format(row.date,row.hour,row.minute,row.second)
            self.reached_stop_loss = True
            failure_count += 1

    def load_exit_price(self):
        self.exit_bid = None
        sql = sql_exit_price.format(currency_pair, self.date,self.expiration_hour)
        for row in tools.dbtools.execute_query(sql):
            self.exit_bid = row.bid

    def execute(self):
        sql = sql_select_ask_price.format(currency_pair, self.date, self.hour)
        entering_price = None
        for row in tools.dbtools.execute_query(sql):
            entering_price = row.ask

        self.take_profit_price = entering_price + self.take_profit_pips / 10000.0
        self.stop_loss_price = entering_price - self.stop_loss_pips / 10000.0

        self.load_take_profit_point()
        self.load_stop_loss_point()

        if self.reached_stop_loss and not self.reached_take_profit:
            exiting_price = self.stop_loss_bid
        elif self.reached_take_profit and not self.reached_stop_loss:
            exiting_price = self.profit_bid
        elif self.reached_take_profit and self.reached_stop_loss:
            if self.stop_loss_date < self.profit_date:
                exiting_price = self.stop_loss_bid
            else:
                exiting_price = self.profit_bid
        else:
            self.load_exit_price()
            if self.exit_bid is not None:
                exiting_price = self.exit_bid
            else:
                return 0

        print exiting_price, entering_price, exiting_price - entering_price
        return (exiting_price - entering_price) * 100000


def read_trades():
    return [ Trade(tokens) for tokens in csv.reader(open(trades_filename))]

if __name__ == '__main__':
     if len(sys.argv) < 2:
        print 'please enter the trades filename and try again'
        sys.exit(0)
     tools.dbtools.db_db="forex"
     currency_pair = sys.argv[1].split('.')[0]
     trades_filename = sys.argv[1]
     trades = read_trades()

     balance = 0
     success_count, failure_count = 0,0
     for t in trades:
        balance += t.execute()

     print success_count, failure_count
     print balance
        



    
