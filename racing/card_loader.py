#!/usr/bin/python

'''
    Creates a race feed that can be used for back testing. 
'''

import os
import csv
import math
import datetime
from random import choice
import pickle
import random
from card import Card
from handicapping_factors import assign_factors
from globals import *



def make_race(track, date, racenumber):
    year = date[0:4]
    month_day = date[4:]
    filename = '{0}/{1}/{2}{3}.DRF'.format(REPOSITORY_PATH, year, track, month_day)
    if not os.path.isfile(filename):
        filename = '{0}/{1}/{2}{3}.MCP'.format(REPOSITORY_PATH, year, track, month_day)
    if not os.path.isfile(filename):
        raise Exception('File: {0} was not found'.format(filename))
    card = Card(filename)
    return card.races[racenumber-1]

    


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
    assign_factors(race)
    print race
    race.handicapping_factor_summary()
