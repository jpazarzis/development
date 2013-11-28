#!/bin/sh
set -x
echo updating

reformat_ticks.py EUR_USD $1
reformat_ticks.py GBP_USD $1
reformat_ticks.py USD_CAD $1
reformat_ticks.py USD_CHF $1
reformat_ticks.py USD_JPY $1

