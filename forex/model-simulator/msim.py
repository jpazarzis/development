#!/usr/bin/python
import sys
from models import *
from tick_feed_manager import get_feed

if __name__ == '__main__':
    feed = get_feed('EUR_USD')

    bid_moves  = [0.0003,0.0005,0.0007,0.001,0.0012, 0.0014, 0.0015,0.0020]
    delta_moves = [0.8, 1.0, 1.2, 1.4, 1.6]

    #bid_moves  = [0.0003,0.001]
    #delta_moves = [0.8, 1.0]

    models = []
    for b1 in bid_moves:
        for b2 in bid_moves:
            for dm in delta_moves:
                m = SpecificMinuteModel(b1,b2,dm)
                feed.register(m)
                m.feed = feed
                models.append(m)

    
    feed.start(5000000)
    for m in models:
        print m

