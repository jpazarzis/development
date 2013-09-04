#!/usr/bin/python
#module: card_repository
import os
import csv
import math
import datetime
from random import choice
from analytics import factor_manager
import pickle
import random
from urllib import urlopen


distances = {990: '4f', 1100: '5f', 1210:'5.5f', 1320:'6f', 1430:'6.5f', 1540:'7f', 1650:'7.5f', 1760:'1m',1799:'*1m40',1800:'1m40', 1830:'1m 70', 1870:'1m 1/16', 1980:'1m 1/8', 2200:'1m 1/4'}

def format_distance(d):
    v =  int(float(d))
    if v not in distances:
        print 'sorry', v, 'not in distances'
    return distances[int(float(d))]




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

        def __repr__(self):
                strg =  '{0:>2}.{1:25}{2:20}{3:3} {4:20}\n'.format(self.program_number, self.name, self.jockey, self.weight, self.trainer)
                if self.past_performances and len(self.past_performances) > 0:
                        for s in self.past_performances:
                            strg += str(s) + '\n'


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

    def is_valid(self):
        return len(self.date.strip()) > 0

    def __repr__(self):
            return  '{0:>10} {1:2} {2:3} {3:10} {4:10} {5:2} {6:12} {7:15} {8:2}'.format(self.date, self.race_number, self.track, self.track_condition, format_distance(self.distance), self.surface, self.race_classification, self.jockey, self.finish_position)



class Race:
    def __init__(self,race_number,parent):
        self.starters = []
        self.number = int(race_number)
        self.parent = parent
        self.exists_in_db = False
        self.race_id = None

    def __repr__(self):
        strg =  'Race number: {0}\n'.format(self.number)
        for s in self.starters:
            strg += str(s) + '\n'

        return strg

class Card:
    def __init__(self,fullpath):
        self.track= get_track_from_fullpath(fullpath)
        self.date=get_date_from_fullpath(fullpath)
        self.races=[]
        race=None
        for tokens in csv.reader(open(fullpath)):
            starter=Starter(tokens)
            assert(starter is not None)
            if not race or race.number != starter.race_number:
                race = Race(starter.race_number,self)
                self.races.append(race)
            starter.parent = race
            race.starters.append(starter)

