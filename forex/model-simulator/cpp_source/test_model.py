#!/usr/bin/python

############################################################################
#
# Author: John Pazarzis
# Date:  Sun Nov  3 10:32:40 EST 2013
# Sample model to be used by PythonDrivenModel
#
############################################################################


# global variable declarations
#
# You can define all your intermediate variables that you need across multiple
# calls as globals and access them from the signal function while you first
# declare them as global

current_hour=-1

triggered_for_current_hour = False

high_for_the_hour = None

enable_logging = False

open_price = 0.0


###################################################################################
#
# Note that the first eight parameters are standard while the last two
# (minute_to_buy, triggering_delta) are specific to the model. 
#
# In the same fashion all your signal functions should follow the same pattern,
# meaning the first 8 parameters should always be the same as here while any
# custom parameters that you need for your logic should be provided in the
# signature of the function after them.
#
#
# The return value should always be an integer having the following values:
#
#   0:  Do nothing
#   
#   1:  BUY
# 
#   2:  SELL
#
###################################################################################

def process_tick(year,month,day, hour, minute,second, bid, ask, minute_to_buy, triggering_delta): 
    global current_hour, triggered_for_current_hour, high_for_the_hour , open_price


    if enable_logging:
        print year,month,day, hour, minute,second, bid, ask, minute_to_buy,\
        triggering_delta, high_for_the_hour, current_hour

    if current_hour != hour:
        # we have entered a new hour, lets update the needed data
        # that's all we need for now....
        current_hour = hour
        open_price = bid
        triggered_for_current_hour = False
        high_for_the_hour = bid
        return 0

    # We are currently within an hour..
    
    # Let's update the high price for the hour if needed
    if bid > high_for_the_hour:
        high_for_the_hour = bid

    # If already triggered or we are not currently in the minute 
    # to trade just exit...
    if  triggered_for_current_hour or minute != minute_to_buy:
        return 0

    delta_in_pips = (high_for_the_hour - open_price) * 10000

    if delta_in_pips < triggering_delta:
        return 0

    # If we reached so far, then it means that the delta is higher
    # than the trigger, so we should send a SELL signal to our caller

    triggered_for_current_hour = True

    return -1


# Use the following for testing only..

if __name__ == '__main__':
    year = 2012
    month = 10
    day = 10
    hour = 5
    minute = 40
    second = 2
    bid = 101.23
    ask = 101.25
    minute_to_buy = 40
    triggering_delta = 12

    signal = process_tick(year,month,day, hour, minute,second, bid, ask, minute_to_buy, triggering_delta)
    print 'signal:', signal
    print 'high_for_the_hour:', high_for_the_hour
    print 'current_hour:', current_hour
    print 'triggered_for_current_hour:', triggered_for_current_hour
