#!/usr/bin/python

import os
import csv
import math
import datetime
from random import choice
import pickle
import random
from card import Card


REPOSITORY_PATH= '/home/john/development/alogatas/horse-racing/Documents/PastPerformances/BrisFiles'

def race_feed():
        ''' returns an iterator to existing races'''
        for year in available_years():
           for filename in cards_for_year(year):
                try:
                    c=Card(filename)
                    for race in c.races:
                        yield race
                except Exception as ex :
                    print ex

def available_years():
    return os.listdir(REPOSITORY_PATH)

def cards_for_year(year):
    path='{0}/{1}'.format(REPOSITORY_PATH,year)
    return  [ path+'/'+x for x in os.listdir(path)]

def get_random_race():
    fn = choice(cards_for_year(choice(available_years())))
    c = Card(fn)
    return random.choice(c.races)

if __name__ == '__main__':
    race = get_random_race()
    print race
