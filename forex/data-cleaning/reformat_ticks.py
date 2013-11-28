#!/usr/bin/python

#. John Pazarzis
# Sun Nov 24 07:14:28 EST 2013

''' 
Reformats a tick file so it can be inserted to mysql
'''
import sys
import csv
import os
import os.path
from os.path import expanduser
from datetime import datetime 

ticks_filename = None

def format_timestamp(ts):
    tokens = ts.split(' ')
    day, month, year = tokens[0].split('/')
    year = int(year)+2000
    return '{0}-{1}-{2},{3}'.format(str(year),month,day, tokens[1].replace(':',','))
    
def compare_timestamps(ts1, ts2):
    tokens = ts1.split(' ')
    day, month, year = tokens[0].split('/')
    ts1 = year + month + day + tokens[1]

    tokens = ts2.split(' ')
    day, month, year = tokens[0].split('/')
    ts2 = year + month + day + tokens[1]
    return ts1 > ts2

def unwrap_date_strg(date_strg):
    tokens = date_strg.split(' ')
    day, month, year =  [ int(t) for t in tokens[0].split('/')]
    year = year + 2000
    hour, minute, second = [ int(t) for t in tokens[1].split(':')]
    return year, month, day, hour, minute, second

def is_weekend(date_strg):
    d = datetime(*unwrap_date_strg(date_strg))
    day_of_week, hour = d.weekday(), d.hour
    if day_of_week == 4:
        return hour >= 17 
    if day_of_week == 5:
        return True
    if day_of_week == 6:
        return hour < 17 
    return False

skipped = 0

def read_ticks():
    time_stamp = ''
    index = 0
    global skipped
    for tokens in csv.reader(open(ticks_filename,'r')):
        if len(time_stamp) and compare_timestamps(time_stamp, tokens[0]):
            print 'skipping:', time_stamp, tokens[0]
            skipped += 1
            continue

        if is_weekend(tokens[0]):
            continue

        if len(time_stamp) == 0 or time_stamp != tokens[0]:
            index = 0
            time_stamp = tokens[0]
        else:
            index += 1
        yield format_timestamp(tokens[0]), str(index), tokens[1],tokens[2]

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'please provide the currency pair and password in the command line and try again'
        sys.exit(1)

    ticks_filename = '{0}/projects/forex/historical-ticks/{1}.csv'.format(expanduser("~"),sys.argv[1])

    if not os.path.exists(ticks_filename):
        print 'sorry, hitorical ticks file:', ticks_filename, 'does not exist...'
        sys.exit(1)

    try:
            # First, lets transform the oanda tick file to a mysql friendly format
            # note that all ticks from Friday 5:00pm to Sunday 5:00pm will be skipped!
            output_filename = '{0}_TRANSFORMED.csv'.format(sys.argv[1])
            f = open(output_filename,'w')
            for i, tick in enumerate(read_ticks()):
                f.write(','.join(tick))
                f.write('\n')
                if i > 100000:
                    break

                if i % 1000000 == 0:
                    print i
            print 'skipped:',skipped

            # Now we are ready to import the data to the data base...
            with open('import_data_template.sql', 'r') as content_file:
                content = content_file.read()

            content = content.replace('CURRENCY_PAIR',sys.argv[1])
            # create a temporary file containing the sql script to update the db

            temp_filename = 'temp_update_{0}.sql'.format(sys.argv[1])
            
            f = open(temp_filename,'w')
            f.write(content)
            f.close()
            arguments = 'mysql --local-infile -uroot -p{0} < {1}'.format(sys.argv[2],temp_filename)

            # go ahead and execute the sql script!
            os.system(arguments)
            sys.exit(0)
       
    except Exception as e:
        print e
        sys.exit(1)


