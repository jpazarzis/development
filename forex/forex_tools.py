#!/usr/bin/python

import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import scipy.stats
import numpy

def plot_curve(values, ylabel='y-values'):
        plt.plot(values)
        plt.ylabel(ylabel)
        plt.show()

def simple_histogram(values, number_of_bins = 10, bar_color='blue',xlabel='x', ylabel='y', title='histogram'):
        plt.hist(values, bins =number_of_bins, color=bar_color)
        plt.show()

def plot_histogram(values, number_of_bins = 10, bar_color='blue',xlabel='x', ylabel='y', title='histogram'):
        n, bins, patches =plt.hist(values, bins =number_of_bins, color=bar_color)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        mean = numpy.average(values)   
        std = numpy.std(values)
        plt.title(r'{0}: mean: {1}, stdev: {2}'.format(title,mean,std))

        plt.subplots_adjust(left=0.15)
        plt.show()

def plot_histogram_from_file(filename, number_of_bins = 10, bar_color='blue',xlabel='x', ylabel='y', title='histogram'):
    values = [float(x) for x in open(filename)]
    plot_histogram(values, number_of_bins , bar_color,xlabel, ylabel, title)
        
def show_stats(filename):
    values = [float(x) for x in open(filename)]
    print 'mean: {0} std: {1}'.format(numpy.average(values), numpy.std(values))


if __name__ == '__main__':
    import random
    x = [random.random() for i in range(10000)]
    plot_histogram(x)
