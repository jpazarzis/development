#!/usr/bin/python   
distances = {990: '4f', 1100: '5f', 1210:'5.5f', 1320:'6f', 1430:'6.5f', 1540:'7f', 1650:'7.5f', 1760:'1m',1799:'*1m40',1800:'1m40', 1830:'1m 70', 1870:'1m 1/16', 1980:'1m 1/8', 2200:'1m 1/4'}

months = [ 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun','Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec' ]

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

