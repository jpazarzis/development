import math
import sys
import random

SHORT_LAYOFF_DAYS = 45
LONG_LAYOFF_DAYS = 120

top_jockeys = [ 'DOMINGUEZ R A', 
                'CASTELLANO A JR', 
                'VELAZQUEZ J R',
                'BEJARANO R',
                'ROSARIO JOEL',
                'LEZCANO JOSE',
                'LEPAROUX JULIEN R',
                'TALAMO JOSEPH',
                'ALVARADO R JR',
                'CONTRERAS LUIS'
               ]

top_trainers = ['PLETCHER TODD A',
                'BAFFERT BOB',
                'ASMUSSEN STEVEN M',
                'ROMANS DALE',
                'BROWN CHAD C',
                'MOTT WILLIAM I',
                'CASSE MARK',
                "O'NEILL DOUG",
                'HOLLENDORFER JERRY',
                'DUTROW ANTHONY W',
                'DUTROW RICHARD E',
                'MCLAUGHLIN KIARAN P',
                'MAKER MICHAEL J',
                'NESS JAMIE',
                'SADLER JOHN W'
            ]



factors = None



def assign_factors(race):
    for starter in race:
        evaluate(starter)


def evaluate(starter):
    global factors

    if not hasattr(starter, 'matching_factors'):
        starter.matching_factors = []

    if not factors:
        factors = get_all()


    for f in [mf for mf in factors if mf(starter)]:
        starter.matching_factors.append(f)



def get_all():
    ff=[]
    module = sys.modules[__name__]
    for func_name in dir(module):
        if func_name.startswith('_') or  func_name.startswith('get_all') or  func_name.startswith('assign_factors') or func_name.startswith('evaluate')  :
            continue
        f = getattr(module,func_name)
        if not callable(f):
            continue
        ff.append(f)
    flag = 1
    for f in ff:
        f.flag = flag
        flag *= 2

    return ff




def claimed_in_last_race(starter):
    try:
        if len(starter.past_performances) == 0:
            return False
        return starter.past_performances[0].claimed_code == 'c'
    except:
        return False


def top_trainer(starter):
    try:
        return starter.trainer in top_trainers
    except:
        return False

def top_jockey(starter):
    try:
        return starter.jockey in top_jockeys
    except:
        return False
    

def superior_speed_figure(starter):

    # the requirements are the following:
    # 1) all starters on the race have at least three races with valid speed ratings
    # 2) one of the three last races of the favorite is a least 4 points more
    #    than any of the last five valid figures of all of the starters of the race

    def get_highest_figure(starter1):
        # returns a tuple of the number of valid figures and the best figure
        figures = []
        for pp in starter1.past_performances:
            try:
                f = int(pp.bris_speed_rating)
                figures.append(f)
            except:
                pass
            if len(figures) >= 3:
                break

        if len(figures) == 0:
            return 0,0

        return len(figures), max(figures)

    c, best_fig_for_favorite = get_highest_figure(starter)

    if c < 3 :
        return False

    for s in starter.parent.starters:
        if s is starter:
            continue
        c,f = get_highest_figure(s)
        if c < 3 or f + 4 >= best_fig_for_favorite:
            return False

    return True
     
    

def blinkers_off(starter):
    try:
        return int(starter.equipment_change) == 2
    except:
        return -1

def blinkers_on(starter):
    try:
        return int(starter.equipment_change) == 1
    except:
        return -1

def no_blinkers_change(starter):
    try:
        return int(starter.equipment_change) == 0
    except:
        return -1


def first_time_out(starter):
    """first time out"""
    return int(starter.life_time_starts) == 0


def short_layoff(starter):
    '''short_layoff'''
    try:
        if first_time_out(starter) or long_layoff(starter): return False
        days_off = int(starter.days_off)
        return LONG_LAYOFF_DAYS >= days_off >= SHORT_LAYOFF_DAYS
    except:
        return False


def layoff(starter):
    '''layoff'''
    try:
        if first_time_out(starter) or long_layoff(starter): return False
        days_off = int(starter.days_off)
        return days_off >= SHORT_LAYOFF_DAYS
    except:
        return False

def long_layoff(starter):
    '''long_layoff'''
    try:
        days_off = int(starter.days_off)
        return days_off >= LONG_LAYOFF_DAYS
    except:
        return False

def second_of_layoff(starter):
    try:
        if first_time_out(starter) or layoff(starter) or long_layoff(starter): return False
        days_offor_last_race = int(starter.past_performances[0].days_off)
        return days_offor_last_race >= SHORT_LAYOFF_DAYS
    except:
        return False    

def third_of_layoff(starter):
    try:
        if first_time_out(starter) or layoff(starter) or long_layoff(starter) or second_of_layoff(starter): return False
        days_offor_previous_to_last_race = int(starter.past_performances[1].days_off)
        return days_offor_previous_to_last_race >= SHORT_LAYOFF_DAYS
    except:
        return False    
        
def deep_form_cycle(starter):
    try:
        if first_time_out(starter): return False
        if layoff(starter): return False
        if long_layoff(starter): return False
        if second_of_layoff(starter): return False
        if third_of_layoff(starter): return False
        return True
    except:
        return False




def run_over_par_lifewise(starter):
    '''over_par_lifewise'''
    try:
        if first_time_out(starter):
            return False
        par=float(starter.speed_par_for_todays_race)
        best_figure_for_life = float(starter.best_figure_for_life)
        return best_figure_for_life > par
    except:
        return False

def run_over_par_this_year(starter):
    '''over_par_this_year'''
    try:
        if first_time_out(starter):
            return False
        par=float(starter.speed_par_for_todays_race)
        best_figure_this_year = float(starter.best_figure_this_year)
        return best_figure_this_year >par
    except:
        return False

def run_over_par_previous_year(starter):
    '''over_par_previous_year'''
    try:
        if first_time_out(starter):
            return False
        par=float(starter.speed_par_for_todays_race)
        best_figure_previous_year = float(starter.best_figure_previous_year)
        return best_figure_previous_year > par
    except:
        return False

def run_over_par_this_track(starter):
    '''over_par_this_track'''
    try:
        if first_time_out(starter):
            return False
        par=float(starter.speed_par_for_todays_race)
        best_figure_this_track = float(starter.best_figure_this_track)
        return best_figure_this_track > par
    except:
        return False


def showed_something_recently(starter):
    '''showed_something''' 
    try:
        for i, pp in enumerate(starter.past_performances):
            if i >=3:
                return False
            second, stretch, finish = pp.get_positions_per_call()
            if second <=4 or stretch <=4 or finish <=4:
                return True
            second, stretch, finish = pp.get_lengths_behind_per_call()
            if second <=2.5 or stretch <=2.5 or finish <=2.5:
                return True
        return False
    except:
        return False



# distance and surface

def first_time_on_distance(starter):
    return int(starter.todays_distance_starts) == 0

def won_on_the_distance(starter):
    return int(starter.todays_distance_wins) >= 1

def second_best_on_the_distance(starter):
    if won_on_the_distance(starter): return False
    return int(starter.todays_distance_places) >= 1

def less_than_3_times_on_distance(starter):
    return 1 <= int(starter.todays_distance_starts) <=3

def less_than_10_times_on_distance(starter):
    return 4 <= int(starter.todays_distance_starts) <=10

def more_than_10_times_on_distance(starter):
    return int(starter.todays_distance_starts) > 10





# class

def entered_for_claim(starter):
    try:
        return float(starter.claiming_price_of_horse) > 0 
    except:
        return False

def entered_for_claim_first_time(starter):
    try:
        if not entered_for_claim(starter):
            return False

        if not starter.past_performances:
            return False
        
        for pp in starter.past_performances:
            cp = pp.claiming_price.strip()
                        
            if cp and float(pp.claiming_price) > 0 :
                return False

        return True
    except:
        return False



def just_broke_the_maiden(starter):
    try:
        return starter.past_performances[0].finish_position == 1 and int(starter.life_time_wins) ==  1
    except:
        return False


def class_drop(starter):
    try:
        tp= _todays_par(starter)
        for i, pp in enumerate(starter.past_performances):
            if i >=3: break
            if _par_for_pp(pp) - tp >= 3:
                return True
        return False

    except:
        return False


def class_step_up(starter):
    try:
        tp= _todays_par(starter)
        for i, pp in enumerate(starter.past_performances):
            if i >=3: break
            if tp - _par_for_pp(pp)  >= 3:
                return True
        return False

    except:
        return False

def same_class(starter):
    return (not class_drop(starter)) and (not class_step_up(starter))


def won_against_better(starter):
    try:
        tp= _todays_par(starter)
        for i, pp in enumerate(starter.past_performances):
            if _was_the_winner(pp) and _par_for_pp(pp) - tp  >= 3 :
                return True
        return False
    except:
        return False

def second_against_better(starter):
    try:

        if won_against_better(starter):
            return False
        tp= _todays_par(starter)
        for i, pp in enumerate(starter.past_performances):
            if int(pp.finish_position) == 2 and _par_for_pp(pp) - tp  >= 3 :
                return True
        return False
    except:
        return False

def MSW_to_MCL_first_time(starter):
    try:
        todays_condition = starter.get_race_condition()
        if  todays_condition != 'MCL':
            return False
        for pp in starter.past_performances:
            if pp.get_race_condition() == 'MCL':
                return False
        return True
    except:
        return False

def MCL_to_MSW_first_time(starter):
    try:
        todays_condition = starter.get_race_condition()
        if  todays_condition != 'MSW':
            return False
        for pp in starter.past_performances:
            if pp.get_race_condition() != 'MCL':
                return False
        return True
    except:
        return False

def entered_for_claim_in_optional(starter):
        todays_condition = starter.get_race_condition()
        if  todays_condition != 'OPTIONAL CLAIMER':
            return False
        return entered_for_claim(starter)

def not_entered_for_claim_in_optional(starter):
        todays_condition = starter.get_race_condition()
        if  todays_condition != 'OPTIONAL CLAIMER':
            return False
        return not entered_for_claim(starter)


def won_last(starter):
    try:
        pp = starter.past_performances[0]
        return int(pp.finish_position) == 1
    except:
        return False

def won_two_ago(starter):
    try:
        if won_last(starter):
            return False
        pp = starter.past_performances[1]
        return int(pp.finish_position) == 1
    except:
        return False

def won_three_ago(starter):
    try:
        if won_last(starter) or won_two_ago(starter):
            return False
        pp = starter.past_performances[2]
        return int(pp.finish_position) == 1
    except:
        return False








# factors about favoritism of the horse

def beaten_favorite_last_time(starter):
    try:
        pp = starter.past_performances[0]
        favorite_flag = int(pp.favorite_indicator) == 1
        finish_position = int(pp.finish_position)
        return favorite_flag and finish_position != 1
    except:
        return False

def beaten_favorite_since_last_win(starter):
    try:
        for pp in starter.past_performances:
            favorite_flag = int(pp.favorite_indicator) == 1
            finish_position = int(pp.finish_position)
            if finish_position == 1:
                return False
            if favorite_flag and finish_position != 1:
                return True
        return False
    except:
        return False


def won_as_favorite_last_race(starter):
    try:
        pp = starter.past_performances[0]
        favorite_flag = int(pp.favorite_indicator) == 1
        finish_position = int(pp.finish_position)
        return favorite_flag and finish_position == 1
    except:
        return False

def last_win_was_favorite(starter):
    try:
        for pp in starter.past_performances:
            favorite_flag = int(pp.favorite_indicator) == 1
            finish_position = int(pp.finish_position)
            if finish_position == 1:
                return favorite_flag
        return False
    except:
        return False

def started_as_favorite_sometime(starter):
    try:
        for pp in starter.past_performances:
            favorite_flag = int(pp.favorite_indicator) == 1
            if favorite_flag:
                return True
        return False
    except:
        return False



# changes / weight

def first_time_lasix(starter):
    try:
        if first_time_out(starter): return False
        return int(starter.medication) == 4
    except:
        return False
    




# pars

#testing factors

def top_two_primer_power(starter):
    try:
        starters = starter.parent.starters[:]
        starters.sort(key = lambda s : s.prime_power, reverse = True)
        return starter in starters[0:2]
    except:
        print 'error here1'
        return False

def top_three_primer_power(starter):
    try:
        starters = starter.parent.starters[:]
        starters.sort(key = lambda s : s.prime_power, reverse = True)
        return starter in starters[0:3]
    except:
        print 'error here2'
        return False

def top_primer_power(starter):
    try:
        starters = starter.parent.starters[:]
        starters.sort(key = lambda s : s.prime_power, reverse = True)
        return starter is starters[0]
    except:
        print 'error here3'
        return False


    
# some utilities
def _todays_par(starter):
    return int(starter.speed_par_for_todays_race)

def _par_for_pp(pp):
    return int(pp.bris_par_for_the_race)
    
def _was_the_winner(pp):
    return int(pp.finish_position) == 1






