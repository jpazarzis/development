#!/usr/bin/python
'''
Author: John Pazarzis, 
Date: Tue Nov 26 09:01:58 EST 2013

use this program to create the input to the neural network
'''

import tools.dbtools as  dbt
import sys

sql_select_candles = '''select date, hour, open, high, low, close 
                        from EUR_USD_CANDLES  where 
                        (date = '{0}' and hour <={1}) or date < '{0}' order by date desc, hour desc LIMIT 24;'''

sql_select_max_price = '''select max(bid) as 'max_bid' from EUR_USD where date = '{0}' and hour >={1} and hour <={2}'''

sql_select_open_price = '''select bid from EUR_USD where date = '{0}' and hour >={1} LIMIT 1;'''

sql_select_weekends = '''select distinct(date), CONCAT(date, ' ', '00:20:00') as d1, CONCAT(ADDDATE(date, 2), ' ', '00:20:00') as d2  from EUR_USD_CANDLES where DAYOFWEEK(date) = 6'''


sql_delete_weekends = '''DELETE FROM EUR_USD WHERE date between DATE('{0}') AND DATE('{1}')'''
sql_delete_weekend_candles = '''DELETE FROM EUR_USD_CANDLES WHERE date between DATE('{0}') AND DATE('{1}')'''


def delete_weekend_rows():
    for i, row in enumerate(dbt.execute_query(sql_select_weekends)):
        print i,row.d1, row.d2
        dbt.execute_non_query(sql_delete_weekends, row.d1, row.d2)
        
def delete_weekend_candles_rows():
    for i, row in enumerate(dbt.execute_query(sql_select_weekends)):
        print i,row.d1, row.d2
        dbt.execute_non_query(sql_delete_weekend_candles, row.d1, row.d2)
        print 'ok'
        
    


if __name__ == '__main__':

    dbt.db_db="forex"
    delete_weekend_candles_rows()
    sys.exit(0)


    for row in dbt.execute_query(sql_select_candles,'2012-10-11',8):
            print row.date, row.hour, row.open, row.high, row.low, row.close

    max_price = dbt.execute_query(sql_select_max_price,'2012-10-11',8,20).next().max_bid
    open_price = dbt.execute_query(sql_select_open_price,'2012-10-11',8).next().bid

    print 'max_price',max_price
    print 'open_price',open_price











