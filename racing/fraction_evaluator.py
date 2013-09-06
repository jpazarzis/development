#!/usr/bin/python

import csv
import sys
from tools import colors


distance_metrics = None

class Enum(set):
    def __getattr__(self, name):
        if name in self:
            return name
        raise AttributeError


FIGURE = Enum(('very_slow', 'slow', 'average', 'fast', 'very_fast'))


class Figurable:
    def __init__(self, slow, average, fast):
        self.slow = float(slow)
        self.average = float(average)
        self.fast = float(fast)

    def __repr__(self):
        return '{0:5.2f} {1:5.2f} {2:5.2f}'.format(self.slow, self.average, self.fast)

    def get_figure(self, value):
        value = float(value)
        if value >= self.slow :
            return FIGURE.very_slow
        elif value > self.slow:
            return FIGURE.slow
        elif value == self.average:
            return FIGURE.average
        elif value < self.fast:
            return FIGURE.very_fast
        else:
            return FIGURE.fast

    def __call__(self, value):
        return self.get_figure(value)


class DistanceMetric:
    def __init__(self,tokens):
        self.track = tokens[0].strip()
        self.distance = tokens[1].strip()
        self.surface = tokens[2].strip()
        self.final_time = Figurable(tokens[3],tokens[4],tokens[5])
        self.opening_call = Figurable(tokens[6],tokens[7],tokens[8])
        self.closing = Figurable(tokens[9],tokens[10],tokens[11])

    def __repr__(self):
        return 'DistanceMetric-> track: {0} distance: {1} surface: {2}'.format(self.track, self.distance, self.surface)


def fancy_format(m, value):
        if m == FIGURE.very_slow:
            return colors.GRAY + value + colors.ENDC
        elif m == FIGURE.slow:
            return colors.BLUE + value + colors.ENDC
        elif m == FIGURE.average:
            return colors.GREEN + value + colors.ENDC
        elif m == FIGURE.fast:
            return colors.RED + value + colors.ENDC
        elif m == FIGURE.very_fast:
            return colors.HRED + colors.RED + value + colors.ENDC
        else:
            return value

class DistanceMetrics:
    def __init__(self, filename='/home/john/development/alogatas/horse-racing/race_info/fractions.csv'):
            self.distance_metrics = {}
            for i, tokens in enumerate(csv.reader(open(filename))):
                if i == 0:
                    continue
                dm = DistanceMetric(tokens)
                key = (dm.track, dm.distance, dm.surface)
                self.distance_metrics[key] = dm

    def get_fractions_for_starter(self,pp):
        
        try:
            finish_beaten_lengths_only = pp.finish_beaten_lengths_only.strip()
            if not finish_beaten_lengths_only:
                finish_beaten_lengths_only = "0"

            second_call_lengths = pp.second_call_beaten_lengths_only.strip()
            if not second_call_lengths:
                second_call_lengths = "0"
            
            finish_position = pp.finish_position
            second_call_position = pp.second_call_position
            
            s = '{0:>2} {1:>2} '.format(second_call_position, finish_position)
            s += '{0:>5} '.format(finish_beaten_lengths_only)
           
            try:
                diff = float(second_call_lengths) - float(finish_beaten_lengths_only)
                if diff>= 0:
                    s += colors.RED
                else:
                    s += colors.GRAY

                s += '{0:>5} '.format(abs(diff))
                s += colors.ENDC
            except:
                pass
            return s

        except:
            return '*'

    def get_closing_time_for_starter(self,pp):
        try:
            finish_beaten_lengths_only = pp.finish_beaten_lengths_only.strip()
            if not finish_beaten_lengths_only:
                finish_beaten_lengths_only = "0"

            second_call_lengths = pp.second_call_beaten_lengths_only.strip()
            if not second_call_lengths:
                second_call_lengths = "0"

            finish_beaten_lengths_only = float(finish_beaten_lengths_only)
            second_call_lengths = float(second_call_lengths)

            finish_position = float(pp.finish_position)
            if finish_position >= 5 or finish_position == 0:
                return '*'
            if finish_beaten_lengths_only > second_call_lengths :
                return '*'
            final_time = float(pp.final_time) + finish_beaten_lengths_only * 0.2
            four_fulrongs_fraction = float(pp.four_fulrongs_fraction) - second_call_lengths * 0.2
            closing_time = final_time - four_fulrongs_fraction
            return closing_time
        except:
            return '*'

    def evaluate_closing_for_starter(self,pp):
        try:
            key = (pp.track.strip(), pp.distance.strip(), pp.surface.strip())
            distance_metric = self.distance_metrics[key]
            closing_time = self.get_closing_time_for_starter(pp)
            closing_metric = distance_metric.closing
            return closing_metric(closing_time)
        except:
            return 'N/A'

    def evaluate_final_time(self, track, surface, distance, final_time):
        try:
            key = (track.strip(), distance, pp.surface.strip())
            distance_metric = self.distance_metrics[key]
            final_time_metric = distance_metric.final_time
            return final_time_metric(final_time)
        except:
            return 'N/A'

    def evaluate_final_time_of_race(self,pp):
        try:
            key = (pp.track.strip(), pp.distance.strip(), pp.surface.strip())
            distance_metric = self.distance_metrics[key]
            final_time = float(pp.final_time)
            final_time_metric = distance_metric.final_time
            return final_time_metric(final_time)
        except:
            return 'N/A'
    
    def evaluate_opening_call_of_race(self,pp):
        try:
            key = (pp.track.strip(), pp.distance.strip(), pp.surface.strip())
            distance_metric = self.distance_metrics[key]
            four_fulrongs_fraction = float(pp.four_fulrongs_fraction)
            opening_call_metric = distance_metric.opening_call
            return opening_call_metric(four_fulrongs_fraction)
        except:
            return 'N/A'


def get_evaluator():
    global distance_metrics
    if  distance_metrics is None:
        distance_metrics = DistanceMetrics()
    assert(None is not distance_metrics)
    return distance_metrics

def evaluate_opening_call_of_race(pp):
    return get_evaluator().evaluate_opening_call_of_race(pp)

def evaluate_final_time_of_race(pp):
    return get_evaluator().evaluate_final_time_of_race(pp)

def evaluate_closing_for_starter(pp):
    return  get_evaluator().evaluate_closing_for_starter(pp)

def get_closing_time_for_starter(pp):
    return  get_evaluator().get_closing_time_for_starter(pp)

def get_fractions_for_starter(pp):
    return  get_evaluator().get_fractions_for_starter(pp)

if __name__ == '__main__':
    from card_loader import race_feed
    for race in race_feed():
        for starter in race.starters:
            for pp in starter.past_performances:
                print fancy_format(evaluate_opening_call_of_race(pp), 'OPEN'),
                print evaluate_opening_call_of_race(pp),
                print fancy_format(evaluate_final_time_of_race(pp), 'FINAL'),
                print evaluate_final_time_of_race(pp),
                print evaluate_closing_for_starter(pp)
