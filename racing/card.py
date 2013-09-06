#!/usr/bin/python

'''
    Constructs a Card object given a bris past performances file. 
'''


#module: card_repository

import os
import csv
import math
import datetime
from random import choice
from analytics import factor_manager
import pickle
import random
from tools import colors
from urllib import urlopen
from fraction_evaluator import *
import race_results 


distances = {990: '4f', 1100: '5f', 1210:'5.5f', 1320:'6f', 1430:'6.5f', 1540:'7f', 1650:'7.5f', 1760:'1m',1799:'*1m40',1800:'1m40', 1830:'1m 70', 1870:'1m 1/16', 1980:'1m 1/8', 2200:'1m 1/4'}

months = [ 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun','Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec' ]


def get_race_condition_from_equibase(equibase_condition):
    equibase_condition=equibase_condition.strip()
    if 'OC' in equibase_condition: return 'OPTIONAL CLAIMER'
    if 'Clm' in equibase_condition: return 'CLAIMER'
    if 'Alw' in equibase_condition or 'Hcp' in equibase_condition: 
        if equibase_condition[-1] == 's':
            return 'STARTER ALLOWANCE'
        else:
            return 'ALLOWANCE'
    if equibase_condition.endswith('-G1'):
        return 'GRADED'
    if equibase_condition.endswith('-G2'):
        return 'GRADED'
    if equibase_condition.endswith('-G3'):
        return 'GRADED'
    if equibase_condition.startswith('G1'):
        return 'GRADED'
    if equibase_condition.startswith('G2'):
        return 'GRADED'
    if equibase_condition.startswith('G3'):
        return 'GRADED'
    if equibase_condition.startswith('GR'):
        return 'GRADED'
    if equibase_condition.endswith('k'):
        return 'STAKES'
    if 'MdSpWt' in equibase_condition: return 'MSW'
    if 'Md' in equibase_condition: return 'MCL'
    if 'Moc' in equibase_condition: return 'MCL'
    if 'Stk' in equibase_condition:
        return 'STAKES'
    return 'STAKES'


def format_time(t):
    try:
            t = float(t)
            i = int(t)

            fifths = ((t - i) *10)/ 2
            fifths = int(fifths+ 0.001)
           
            if i < 60: 
                return '   {0:>2}.{1}'.format(i,fifths)
            else:
                minutes = i / 60
                seconds = i % 60
                return ' {0:1}:{1:02d}.{2}'.format(minutes,seconds,fifths)
    except:
        return t



def format_distance(d):
    v =  int(float(d))
    prefix = ''
    if v < 0:
        v = -1 * v
        prefix = '*'

    if v not in distances:
        return d

    return prefix+distances[v]

def format_date(d):

    year = d[2:4]
    month = months[int(d[4:6])-1]
    day = d[6:]
    return '{0}{1}{2}'.format(day,month,year)





starter_data_map = {
        "track" : 0 ,
        "date" : 1 ,
        "race_number" : 2 ,
        "post_position" : 3 ,
        "entry_indicator" : 4 ,
        "todays_distance" : 5 ,
        "todays_surface" : 6 ,
        "race_type" : 8 ,
        "age_sex_restrictions" : 9 ,
        "race_classification" : 10 ,
        "purse" : 11 ,
        "race_conditions" : 15 ,
        "all_weather_flag" : 24 ,
        "jockey" : 32 ,
        "jockey_starts" : 34 ,
        "jockey_wins" : 35 ,
        "owners_name" : 38 ,
        "owners_silks" : 39 ,
        "medication" : 61 ,
        "todays_medication" : 62 ,
        "equipment_change" : 63 ,
        "program_number" : 42 ,
        "odds" : 43 ,
        "name" : 44 ,
        "year_of_birth" : 45 ,
        "foaling_month" : 46 ,
        "horse_sex" : 48 ,
        "horse_color" : 49 ,
        "weight" : 50 ,
        "sire" : 51 ,
        "sires_sire" : 52 ,
        "dam" : 53 ,
        "dams_sire" : 54 ,
        "state_where_was_bred" : 56 ,
        "updated_post_position" : 57 ,
        "sires_stud_fee" : 1176 ,
        "life_time_starts" : 96 ,
        "life_time_wins" : 97 ,
        "life_time_places" : 98 ,
        "life_time_shows" : 99 ,
        "life_time_earnings" : 100 ,
        "current_year" : 84 ,
        "current_year_starts" : 85 ,
        "current_year_wins" : 86 ,
        "current_year_places" : 87 ,
        "current_year_shows" : 88 ,
        "current_year_earnings" : 89 ,
        "previous_year" : 90 ,
        "previous_year_starts" : 91 ,
        "previous_year_wins" : 92 ,
        "previous_year_places" : 93 ,
        "previous_year_shows" : 94 ,
        "previous_year_earnings" : 95 ,
        "todays_track_starts" : 69 ,
        "todays_track_wins" : 70 ,
        "todays_track_places" : 71 ,
        "todays_track_shows" : 72 ,
        "todays_track_earnings" : 73 ,
        "wet_track_starts" : 79 ,
        "wet_track_wins" : 80 ,
        "wet_track_places" : 81 ,
        "wet_track_shows" : 82 ,
        "wet_track_earnings" : 83 ,
        "turf_track_starts" : 74 ,
        "turf_track_wins" : 75 ,
        "turf_track_places" : 76 ,
        "turf_track_shows" : 77 ,
        "turf_track_earnings" : 78 ,
        "todays_distance_starts" : 64 ,
        "todays_distance_wins" : 65 ,
        "todays_distance_places" : 66 ,
        "todays_distance_shows" : 67 ,
        "todays_distance_earnings" : 68 ,
        "bris_run_style" : 209 ,
        "quirin_speed_points" : 210 ,
        "speed_par_for_todays_race" : 216 ,
        "days_off" : 223 ,
        "todays_race_state_bred" : 238 ,
        "claiming_price_of_race" : 12 ,
        "claiming_price_of_horse" : 13 ,
        "prime_power" : 250 ,
        "trainer" : 27 ,
        "trainer_starts" : 28 ,
        "trainer_wins" : 29 ,
        "trainer_places" : 30 ,
        "trainer_shows" : 31 ,
        "max_past_performances" : 10 ,
        "bris_avg_three_last_class_ratings" : 1145 ,
        "bris_dirt_pedigree_rating" : 1263 ,
        "bris_mud_pedigree_rating" : 1264 ,
        "bris_turf_pedigree_rating" : 1265 ,
        "bris_distance_pedigree_rating" : 1266 ,
        "best_figure_for_life" : 1327,
        "best_figure_this_year" : 1328,
        "best_figure_previous_year" : 1329,
        "best_figure_this_track" : 1330,
        "post_time" : 1417,
        "todays_race_equibase_race_condition": 1428,
        'trainer_starts_current_year' : 1146,
        'trainer_wins_current_year' : 1147,
        'trainer_starts_previous_year' : 1151,
        'trainer_wins_previous_year' : 1142,
        'jockey_starts_current_year' : 1156,
        'jockey_wins_current_year' : 1157,
        'jockey_starts_previous_year' : 1161,
        'jockey_wins_previous_year' : 1162
}

pp_data_map = {
    "date" : 255 ,
    "days_off" : 265 ,
    "track" : 275 ,
    "race_number" : 295 ,
    "track_condition" : 305 ,
    "distance" : 315 ,
    "surface" : 325 ,
    "number_of_entrants" : 345 ,
    "post_position" : 355 ,
    "equipment" : 365 ,
    "race_name" : 375 ,
    "medication" : 385 ,
    "weight" : 505 ,
    "trip_comment" : 396 ,
    "winners_name" : 405 ,
    "second_place_finisher_name" : 415 ,
    "third_place_finisher_name" : 425 ,
    "winners_weight" : 435 ,
    "second_place_finisher_weight" : 445 ,
    "third_place_finisher_weight" : 455 ,
    "extra_comment_line" : 495 ,
    "odds" : 515 ,
    "race_classification" : 535 ,
    "claiming_price" : 545 ,
    "purse" : 555 ,
    "start_call_position" : 565 ,
    "first_call_position" : 575 ,
    "second_call_position" : 585 ,
    "stretch_position" : 605 ,
    "finish_position" : 615 ,
    "money_position" : 625 ,
    "first_call_lengths" : 655 ,
    "second_call_lengths" : 675 ,
    "stretch_lengths" : 715 ,
    "finish_lengths" : 735 ,
    "first_call_beaten_lengths_only" : 665 ,
    "second_call_beaten_lengths_only" : 685 ,
    "stretch_call_beaten_lengths_only" : 725 ,
    "finish_beaten_lengths_only" : 745 ,
    "bris_2_furlong_pace_figure" : 765 ,
    "bris_4_furlong_pace_figure" : 776 ,
    "bris_race_shape_first_call" : 695 ,
    "bris_race_shape_second_call" : 755 ,
    "bris_late_pace_figure" : 815 ,
    "bris_race_rating" : 825 ,
    "bris_class_rating" : 835 ,
    "bris_speed_rating" : 845 ,
    "track_variant" : 865 ,
    "two_fulrongs_fraction" : 875 ,
    "three_fulrongs_fraction" : 885 ,
    "four_fulrongs_fraction" : 895 ,
    "five_fulrongs_fraction" : 905 ,
    "six_fulrongs_fraction" : 915 ,
    "seven_fulrongs_fraction" : 925 ,
    "eight_fulrongs_fraction" : 935 ,
    "ten_fulrongs_fraction" : 945 ,
    "twelve_fulrongs_fraction" : 955 ,
    "forteen_fulrongs_fraction" : 965 ,
    "sixteen_fulrongs_fraction" : 975 ,
    "first_fraction" : 985 ,
    "second_fraction" : 995 ,
    "third_fraction" : 1005 ,
    "final_time" : 1035 ,
    "claimed_code" : 1045 ,
    "age_sex_restrictions" : 1095 ,
    "state_bred_flag" : 1105 ,
    "favorite_indicator" : 1125 ,
    "trainer" : 1055 ,
    "jockey" : 1065 ,
    "race_type" : 1085 ,
    "bris_par_for_the_race":1166 ,
    "max_claiming_price_of_the_race" : 121,
    "equibase_race_condition": 1418,
    "distance_for_first_fraction": 985,
    "distance_for_second_fraction": 995,
    "distance_for_third_fraction": 1005
}

def get_track_from_fullpath(fullpath):
        return os.path.split(fullpath)[1][0:3]

def get_date_from_fullpath(fullpath):
    year = os.path.split(fullpath)[0]
    year = os.path.split(year)[1]
    return year+os.path.split(fullpath)[1][3:7]


class Starter:
        def __init__(self, fields):
                for property_name in starter_data_map.keys():
                    field_index = starter_data_map[property_name]
                    setattr(self, property_name, fields[field_index])
                pp=[]
                for i in range(10):
                    p=PastPerformance(fields,i,self)
                    if not p.is_valid():
                        break;
                    pp.append(p)

                try:
                    self.prime_power = float(self.prime_power)
                except:
                    self.prime_power = 0

                self.race_number = int(self.race_number)
                self.morning_line_odds = self.odds
                self.past_performances=tuple(pp)


        def __iter__(self):
            return iter(self.past_performances)

        def finalize(self):
            for pp in self:
                pp.finalize()

            try:
                self.prime_power = int(float(self.prime_power))
            except:
                self.prime_power =0


        def get_race_condition(self):
            return get_race_condition_from_equibase(self.todays_race_equibase_race_condition)



        def __repr__(self):
                strg =  '{0:>2}.{1:25}{2:20}{3:3} {4:20} {5:>3}\n'.format(self.program_number, self.name, self.jockey, self.weight, self.trainer, self.prime_power)
                    
                if hasattr(self, 'finish_position'):
                    strg += 'finish position: {0} \n'.format(self.finish_position)

                if hasattr(self, 'final_odds'):
                    strg += 'final odds: {0}\n'.format(self.final_odds)


    
                
                if self.past_performances and len(self.past_performances) > 0:
                        for s in self.past_performances:
                            strg += str(s) + '\n'
                
                if hasattr(self, 'matching_factors'):
                    strg += ' Matching Factors \n'
                    for f in self.matching_factors:
                        strg += f.__name__ + '\n'

                return strg

class PastPerformance:
    def __init__(self,tokens,index,parent):
            self.parent=parent
            for property_name in pp_data_map.keys():
                    field_index = pp_data_map[property_name]
                    setattr(self, property_name, tokens[field_index+index])
            try:
                self.finish_position = int(self.finish_position)
            except:
                self.finish_position = 999

            if self.track == 'PRX' :
                self.track = 'PHA'


    def finalize(self):
        # add days ago
        todays_date = self.parent.parent.parent.date
        todays_date = datetime.date(int(todays_date[0:4]),int(todays_date[4:6]),int(todays_date[6:]))
        pp_date  = datetime.date(int(self.date[0:4]),int(self.date[4:6]),int(self.date[6:]))
        self.days_ago = (todays_date - pp_date).days 

        self.opening_call_metric = evaluate_opening_call_of_race(self)
        self.final_time_metric = evaluate_final_time_of_race(self)
        self.closing_time_metric = evaluate_closing_for_starter(self)

        
    def is_valid(self):
        return len(self.date.strip()) > 0

    def __repr__(self):
            s = '{0:>5} '.format(self.days_ago)

            if 'T' in self.surface or 't' in self.surface:
                s += colors.GREEN
            s += '{0:>3} '.format(self.track)
            s += '{0:>2} '.format(self.track_condition)

            if 'T' in self.surface or 't' in self.surface:
                s += colors.ENDC

            s += '{0:15} '.format(self.race_classification)
            s += '{0:10} '.format(format_distance(self.distance))
            s += '{0:5} '.format(self.weight)

            s +=fancy_format(self.opening_call_metric, '{0:10} '.format(format_time(self.four_fulrongs_fraction)))
            s +=fancy_format(self.final_time_metric, '{0:10} '.format(format_time(self.final_time)))
            s +=fancy_format(self.closing_time_metric, '{0:>8} '.format(format_time(get_closing_time_for_starter(self))))
            s+= get_fractions_for_starter(self)

            return s


class Race:
    def __init__(self,race_number,parent):
        self.starters = []
        self.number = int(race_number)
        self.parent = parent
        self.exists_in_db = False
        self.race_id = None
        self.favorite = None

    def __iter__(self):
        return iter(self.starters)


    def retrieve_results(self):
        race_results.retrieve_results(self)

    def handicapping_factor_summary(self):
        factors = {}
        for starter in self.starters:
            if not hasattr(starter, 'matching_factors'):
                continue
            for factor in starter.matching_factors:
                name = factor.__name__
                if name not in factors:
                    factors[name] = 1
                else:
                    factors[name] += 1

        for name in factors:
            print '{0:40} {1:>5}'.format(name, factors[name])
                

    def __repr__(self):
        strg =  'Race number: {0}\n'.format(self.number)

        if self.results_available:
            strg += "RESULTS ARE AVAILABLE\n"

        if hasattr(self,'winner') and self.winner:
            strg +=  'Winner:\n {0}\n'.format(self.winner.name)

        if hasattr(self,'favorite') and self.favorite:
            strg +=  'Favorite:\n {0}\n'.format(self.favorite.name)



        for s in self.starters:
            strg += str(s) + '\n'

        return strg

class Card:
    def __init__(self,fullpath):
        self.track= get_track_from_fullpath(fullpath)
        self.date=get_date_from_fullpath(fullpath)
        self.races=[]
        race=None
        self.results_available = False
        for tokens in csv.reader(open(fullpath)):
            starter=Starter(tokens)
            assert(starter is not None)
            if not race or race.number != starter.race_number:
                race = Race(starter.race_number,self)
                self.races.append(race)
            starter.parent = race
            starter.finalize()
            race.starters.append(starter)
        self.retrieve_results()

    def __iter__(self):
        return iter(self.races)

    def __getitem__(self, index):
        return self.races[index]

    def retrieve_results(self):
        self.results_available = race_results.card_exists_in_db(self.track,self.date)
        if self.results_available:
            map( lambda x : x.retrieve_results(), self.races)
        

if __name__ == '__main__':
    from globals import *
    c = Card('{0}/2010/AQU0101.MCP'.format(REPOSITORY_PATH))

    print c[2]







