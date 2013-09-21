#!/usr/bin/python

'''
Implements a factor evaluation mechanism that can be applied
to any function that can serve as a handicapping factor.
'''

from card_loader import race_feed
from handicapping_factors import *
from tools import colors
from collections import Counter
import sys
from race_metrics import assign_metrics

class FactorStats:
    def __init__(self, factor_function):
        self.factor_function = factor_function
        self.observed_matching_winners = 0
        self.expected_matching_winners = 0
        self.observed_non_matching_winners = 0
        self.expected_non_matching_winners = 0
        self.total_winnings = 0.0
        self.total_winnings_no_take_out = 0.0
   
    def verify(self):
        observed_total =  self.observed_matching_winners + self.observed_non_matching_winners
        expected_total =  self.expected_matching_winners + self.expected_non_matching_winners
        print 'observed winners: {0:10.2f} expected winners: {1:10.2f}'.format(self.observed_matching_winners, self.expected_matching_winners)
        print 'observed total: {0:10.2f} expected total: {1:10.2f}'.format(observed_total, expected_total)
        print self.observed_matching_winners - self.expected_matching_winners,
        print self.observed_non_matching_winners - self.expected_non_matching_winners

    def chi_square(self):
        def get_factor(observed, expected):
            observed, expected = observed *1.0 , expected *1.0
            diff = abs(observed - expected) - 0.5 # Yates correction
            return (diff ** 2) / expected

        chi = get_factor(self.observed_matching_winners, self.expected_matching_winners)
        chi += get_factor(self.observed_non_matching_winners, self.expected_non_matching_winners)
        return chi
        
    def add(self,race):
        matches = [ starter for starter in race if self.factor_function(starter) ]
        if len(matches) == 0:
            return
        non_matches = [ starter for starter in race if not self.factor_function(starter) ]
        if len(non_matches) == 0:
            return

        assert(len(matches) + len(non_matches) == len(race.starters) )

        if race.winner in matches:
            self.observed_matching_winners += 1
            self.total_winnings += (1.0 + race.winner.final_odds)
            self.total_winnings_no_take_out  += (1.0 + (1 - race.winner.crowd_probability) / race.winner.crowd_probability)

        if race.winner in non_matches:
            self.observed_non_matching_winners += 1

        self.expected_matching_winners += sum( [s.crowd_probability for s in matches])
        self.expected_non_matching_winners += sum( [s.crowd_probability for s in non_matches])

    def show_details(self):
        print self.factor_function.__name__

        if self.expected_matching_winners == 0 or self.expected_non_matching_winners == 0:
            return
        print 'MATCHES  : observed winners: {0:10.2f} expected winners: {1:10.2f}'.format(self.observed_matching_winners, self.expected_matching_winners)
        print 'NOMATCHES: observed winners: {0:10.2f} expected winners: {1:10.2f}'.format(self.observed_non_matching_winners, self.expected_non_matching_winners)
        chi = self.chi_square()
        print 'x2: {0:10.5f}'.format(chi)
        if chi > 3.84:
            print ' significant'
        else:
            print ' NOT significant'
        print '='*40 
        
    def __repr__(self):
        total_starters = self.observed_loosers + self.observed_winners
        if total_starters == 0 :
            return ''
        tokens = []
        tokens.append(self.factor_function.__name__)
        tokens.append('{0:10.2f}'.format(self.expected_winners))
        tokens.append('{0:10.2f}'.format(self.observed_winners))
        tokens.append('{0:10.2f}'.format(self.observed_winners/self.expected_winners))
        tokens.append('{0:10.2f}'.format(self.total_winnings / (total_starters*1.0)))
        return ','.join(tokens)

def analyze(factors):
    factor_stats = [ FactorStats(f) for f in factors]
    count = 0
    for race in race_feed():
        if len([s for s in race if s.final_odds < 0.01 or len(s.entry_indicator.strip()) > 0]) > 0:
                    continue
        try:
                race.takeout = 1.0 - 1.0 / sum( [ 1.0 / (1.0 + s.final_odds) for s in race] )
                for starter in race:
                    starter.crowd_probability = (1.0 - race.takeout) / (1.0 + starter.final_odds)

                map(lambda f : f.add(race), factor_stats)
                count += 1

        except Exception as e:
            print 'here',e, race.parent.track, race.parent.date, race.number
    map(lambda f : f.show_details() , factor_stats)
        
def analyze_by_metric():
    factors_by_metric = {}
    count = 0
    for race in race_feed(2000):
        if len([s for s in race if s.final_odds < 0.01 or len(s.entry_indicator.strip()) > 0]) > 0:
                    continue
        assign_metrics(race)
        for m in race.metrics:
            if m not in factors_by_metric:
                factors_by_metric[m] = [ FactorStats(f) for f in [just_broke_the_maiden]]
        try:
                race.takeout = 1.0 - 1.0 / sum( [ 1.0 / (1.0 + s.final_odds) for s in race] )
                for starter in race:
                    starter.crowd_probability = (1.0 - race.takeout) / (1.0 + starter.final_odds)

                for m in race.metrics:
                    factor_stats = factors_by_metric[m]
                    map(lambda f : f.add(race), factor_stats)
                count += 1
        except Exception as e:
            print 'here',e, race.parent.track, race.parent.date, race.number

    for m in factors_by_metric:
        print m
        factor_stats = factors_by_metric[m]
        map(lambda f : f.show_details() , factor_stats)

def combine_factors(*foos):
    def f(x):
        for foo in foos:
            if not foo(x):
                return False
        return True
    return f
    
if __name__ == '__main__':
    recency_factors = [layoff, long_layoff, second_of_layoff, third_of_layoff, deep_form_cycle]
    performance_factors = [recent_races_are_bad,recent_races_are_good]
    
    factors = []
    for f1 in recency_factors:
        for f2 in performance_factors:
            composite_factor = combine_factors(f1,f2)
            composite_factor.__name__ = '{0}_and_{1}'.format(f1.__name__, f2.__name__)
            factors.append(composite_factor)

    analyze(factors)
