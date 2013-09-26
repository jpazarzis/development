#!/usr/bin/python
from models import *
from tick_feed_manager import get_feed

if __name__ == '__main__':
    feed = get_feed('EUR_USD')
    m = SpecificMinuteModel()
    feed.register(m)
    feed.start(2000000)
    m.show_stats()

    
