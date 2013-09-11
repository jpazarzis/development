#!/usr/bin/python

from card_loader import race_feed
from handicapping_factors import *
from tools import colors
from collections import Counter

#race.takeout = 1.0 - 1.0 / sum( [ 1.0 / (1.0 + s.final_odds) for s in race.starters] )


class FactorStats:
    def __init__(self, factor_function):
        self.factor_function = factor_function

        self.observed_loosers = 0
        self.observed_winners = 0

        self.expected_winners = 0.0
        self.expected_loosers = 0.0

        self.total_winnings = 0.0
        self.total_winnings_no_take_out = 0.0
    

    def chi_square(self):
        def get_factor(observed, expected):
            observed, expected = observed *1.0 , expected *1.0
            return ((observed - expected) ** 2) / expected

        chi = get_factor(self.observed_loosers, self.expected_loosers)
        chi += get_factor(self.observed_winners, self.expected_winners)

        return chi
        

    def add(self,starter):
        if not self.factor_function(starter):
            return

        if starter.finish_position == 1:
            self.observed_winners += 1
            self.total_winnings += (1.0 + starter.final_odds)
            self.total_winnings_no_take_out  += (1.0 + (1 - starter.crowd_probability) / starter.crowd_probability)

        else:
            self.observed_loosers += 1

        self.expected_winners += starter.crowd_probability
        self.expected_loosers += 1.0 - starter.crowd_probability

    def show_details(self):
        print '=' *80   
        print  self.factor_function.__name__ 
        total_starters = self.observed_loosers + self.observed_winners
        if total_starters == 0 :
            return '{0:40}: NON STARTERS'.format(self.factor_function.__name__)
        

        print 'Expected Winners ..... {0:10.2f}'.format(self.expected_winners)
        print 'Observed Winners ..... {0:10.2f}'.format(self.observed_winners)
        print 'ROI .................. {0:10.2f}'.format(self.total_winnings / (total_starters*1.0))
        #print 'chi_square ........... {0:10.2f}'.format(self.chi_square())

    def __repr__(self):
        total_starters = self.observed_loosers + self.observed_winners
        if total_starters == 0 :
            return ''
        s = '{0:35} '.format(self.factor_function.__name__)
        s += '{0:10.2f} '.format(self.expected_winners)
        s += '{0:10.2f}'.format(self.observed_winners)
        s += '{0:10.2f}'.format(self.observed_winners/self.expected_winners)
        s += '{0:10.2f}'.format(self.total_winnings / (total_starters*1.0))
        return s




def all_favorites(starter):
    return starter.parent.favorite is starter

def foo(days_off):
    def x(starter):
        return layoff_by_days(starter, days_off)
    return x        


def tlo(days_off):
    def x(starter):
        return third_of_layoff_by_days(starter, days_off)
    return x        

def slo(days_off):
    def x(starter):
        return second_of_layoff_by_days(starter, days_off)
    return x        



def recency_histogram():
    counter = Counter()
    for race in race_feed():
        if len([s for s in race if s.final_odds < 0.01 or len(s.entry_indicator.strip()) > 0]) > 0:
                    continue
        try:
            for starter in race:
                if first_time_out(starter):
                    pass
                else:
                    counter[int(starter.days_off)] += 1
        except Exception as e:
            print 'here',e, race.parent.track, race.parent.date, race.number

    keys = sorted(counter.keys())
    for k in keys:
        print '{0},{1}'.format(k, counter[k])



def analyze_favorites():
    #factor_stats =[all_favorites, first_time_out, short_layoff, layoff, long_layoff, second_of_layoff, third_of_layoff, deep_form_cycle] 

    #factor_stats = [ FactorStats(f) for f in factor_stats]

    factor_stats = []
 
    for days_off in  range(20,200,15):

        f = foo(days_off)
        f.__name__ = 'layoff_{0}'.format(days_off)
        func = FactorStats(f)
        factor_stats.append(func)

        f = slo(days_off)
        f.__name__ = '2nd of layoff_{0}'.format(days_off)
        func = FactorStats(f)
        factor_stats.append(func)

        f = tlo(days_off)
        f.__name__ = '3rd of layoff_{0}'.format(days_off)
        func = FactorStats(f)
        factor_stats.append(func)


    count = 0

    for race in race_feed():

        if len([s for s in race if s.final_odds < 0.01 or len(s.entry_indicator.strip()) > 0]) > 0:
                    continue

        try:
                race.takeout = 1.0 - 1.0 / sum( [ 1.0 / (1.0 + s.final_odds) for s in race] )
                for starter in race:
                     starter.crowd_probability = 1.0 / ( (race.takeout + 1.0) * ( 1.0 + starter.final_odds))

                map(lambda f : f.add(race.favorite), factor_stats)
                count += 1

        except Exception as e:
            print 'here',e, race.parent.track, race.parent.date, race.number


    s = '{0:35} '.format('factor')
    s += '{0:12} '.format('expected')
    s += '{0:12}'.format('observed')
    s += '{0:12}'.format('rate')
    s += '{0:12}'.format('roi')
    print s
    print


    for factor in factor_stats:
        print str(factor)
        #factor.show_details()

    print 'number of races:', count
    
if __name__ == '__main__':

    recency_histogram()



