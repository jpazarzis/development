#!/usr/bin/python
"""
retrives race results from the data base
"""


from tools.dbtools import *


SQL_GET_RESULTS_FOR_RACE="select id, program_number, odds, finish_position, race_id, favorite_flag from "\
                      "race_starters where track_code = '{0}' and racing_date = '{1}' "\
                      "and race_number={2} and program_number != 'SCR'"

SQL_GET_RACE_DESCRIPTION="select surface,distance,track_condition,race_number, final_time, time_for_fraction_2, fraction_2_distance "\
                     "from race_description where track_code = '{0}' "\
                     "and date_of_the_race = '{1}' "

SQL_CHECK_IF_RACE_EXISTS_IN_DB="select count(*) as counter from race_description where track_code = '{0}' and date_of_the_race = '{1}' "             


def retrieve_results(race):
    race.results_available = False
    track, date = race.parent.track, race.parent.date
    if not card_exists_in_db(track,date):
        return False

    results = get_results('AQU','20100101',1)

    if len(results) == 0:
        return False

    starters_after_scratches = []

    for starter in race:
        pn = starter.program_number
        if pn in results:
            starters_after_scratches.append(starter)
            result = results[pn]
            starter.finish_position = result.finish_position
            starter.final_odds = result.odds
            if starter.finish_position == 1:
                race.winner = starter
            if result.favorite_flag == 1:
                race.favorite = starter

    race.starters = starters_after_scratches
    race.results_available = True

    return True


def get_results(trackcode,date,racenumber):
        results={}
        for row in execute_query(SQL_GET_RESULTS_FOR_RACE,trackcode,date,racenumber):
            results [row.program_number] = row
        return results


def get_descriptive_data(trackcode,date):
         desc = {}
         for row in execute_query(SQL_GET_RACE_DESCRIPTION,trackcode,date):
            desc[row.race_number] = row
         return desc
         
def card_exists_in_db(trackcode,date):
         for row in execute_query(SQL_CHECK_IF_RACE_EXISTS_IN_DB,trackcode,date):
            return row.counter > 0 


def general_test():
            print 'testing race results'
            starters=get_results('AQU','20100101',1)
            print 'number of starters',len(starters)
            for program_number in starters:
                s = starters[program_number]
                print program_number,s.odds, s.finish_position, s.race_id
            dd = get_descriptive_data('AQU','20100101')
            for race_number in dd:
                r = dd[race_number]
                print "race num: {0} surface: {1} distance: {2} track_cond: {3}".format(race_number,r.surface, r.distance, r.track_condition)
            assert(card_exists_in_db('AQU','20100101'))
            assert(not card_exists_in_db('XYZ','20100101'))

def specific_test():
    from card_loader import make_race
    try:
            race = make_race('AQU','20100101',1)
            
            if retrieve_results(race):
                print race
            else:
                print 'results not available'
    except Exception as e:
        print e

if __name__ == '__main__':
    specific_test()
 


