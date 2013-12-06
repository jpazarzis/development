#!/usr/bin/python
'''
John Pazarzis
Fri Nov 29 10:18:54 EST 2013

Given a specific currency pair creates the following:

(1) A training file containing the input/output to the neural network

(2) A testing file to be used to test the results of the NN

(3) The actual neural network file containing its weights

Both the training (1) and the testing (2) files are cleaned for conflicts
'''
import sys
import os
import tools.dbtools

sql_select_entering_hours = '''select date, hour from EUR_USD 
                                   where hour = {0} group by date, hour'''

sql_select_candles = '''select open, high, low, close from EUR_USD_CANDLES 
                            where (date = '{0}' and hour <={1}) 
                                or 
                            date < '{0}' 
                            order by date desc, hour desc
                            LIMIT {2}'''

sql_select_entering_price =''' select ask from  EUR_USD where date='{0}' AND hour = {1} LIMIT 1 '''


# WARNING Please read!
# This statement is a bit tricky... 
# What we try to do here is to select the max ask price for the following N
# hours. 
# In the data base we store date as a YYYY-MM-DD string and hour as an integer,
# this means that if we want to cover the same day we need to select using
# explicitely providing date and hours to an or clause as can be seen in the
# following code.  If you need to cover the next consecutive day you need to
# change the construction of the sql. For now I am only using the following 10
# hours and a long as my entering hour is less than 14:00 (or 2:00pm) I must use
# the same date....

sql_select_max_bid_for_following_hours = '''select max(bid) as bid from EUR_USD WHERE  date = '{0}' AND  hour >= {1} and hour < {2}'''

                                        

def load_entering_hours():
      return [ row for row in tools.dbtools.execute_query(sql_select_entering_hours)] 

def create_training_data(date, hour):
    invalid_return = None,None, None
    # First get the corresponding candles 
    sql = sql_select_candles.format(date, hour,number_of_candles)
    input_vector = []
    for row in tools.dbtools.execute_query(sql):
        input_vector.extend([row.open,row.high,row.low,row.close])

    if len(input_vector) < number_of_candles*4:
        # There are not enough candles. Most likely I am still in the beginning
        # of the data...
        return invalid_return

    # Normalize input_vector
    min_value, max_price = min(input_vector), max(input_vector)
    diff = max_price - min_value
    input_vector = [ (value - min_value) / diff for value in input_vector]

    # at this point I have the input_vector to the neural network, I still to construct
    # its expected output. For this I will select the opening ask price and if
    # the bid price went over the desired spread I will signify the output as 1
    # or 0 otherwise...

    entering_price = None
    sql = sql_select_entering_price.format(date, hour)
    for row in tools.dbtools.execute_query(sql):
        entering_price = row.ask
    assert(entering_price)

    max_reached_bid_price = None
    sql = sql_select_max_bid_for_following_hours.format(date, hour,expiration_hour)
    for row in tools.dbtools.execute_query(sql):
        max_reached_bid_price = row.bid
    if max_reached_bid_price == None:
        # most likely I have reached the end of the data...
        return invalid_return

    diff = (max_reached_bid_price - entering_price) * 10000
    if diff > delta_in_pips:
        output = 1
    else:
         output = 0

    return date, input_vector, output


def create_file(filename, data, is_training_file):
    f = open(filename,'w')
    if is_training_file:
        f.write('{0} {1} 1\n'.format(len(data), number_of_candles * 4))
    for td in data:
        input_data = td[1]
        output_data = td[2]
        for d in input_data:
            s = '{0:10.4f} '.format(d)
            s = s.strip();
            f.write('{0} '.format(s))
        f.write('\n')
        f.write('{0}\n'.format(output_data))
    f.close()

def create_trading_file(filename, data):
    f = open(filename,'w')
    for td in data:
        date = td[0]
        tokens = [date, str(entering_hour), str(expiration_hour), str(delta_in_pips) ]
        input_data = td[1]
        tokens.extend(['{0:10.4f} '.format(d).strip() for d in input_data])
        f.write( '{0}\n'.format(','.join(tokens)))
    f.close()

if __name__ == '__main__':
    tools.dbtools.db_db="forex"
    currency_pair = 'EUR_USD'
    entering_hour = 8
    expiration_hour = 18 # see notes above
    number_of_candles = 24
    delta_in_pips = 40
    sql_select_entering_hours = sql_select_entering_hours.format(entering_hour)
    entering_hours = load_entering_hours()
    all_data = []
    for eh in entering_hours:
        date, input_vector, output_vector = create_training_data(eh.date, eh.hour)
        if input_vector is not None:
            assert(len(input_vector) == number_of_candles * 4)
            all_data.append((date, input_vector, output_vector))

    cutoff = int(len(all_data) * 0.7)
    training_data = all_data[:cutoff]
    testing_data = all_data[cutoff:]
    training_filename = '{0}_training.data'.format(currency_pair)
    create_file(training_filename, training_data, True)
    create_file('{0}_testing.data'.format(currency_pair), testing_data, False)
    create_trading_file('{0}.trading_file'.format(currency_pair), testing_data)
    os.system("data_cleaner.py {0}".format(training_filename))






