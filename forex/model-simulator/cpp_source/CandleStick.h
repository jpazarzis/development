// CandleStick.h
//
// Author        : John Pazarzis
// Creation date : Tue 12 Nov 2013 12:49:34 PM EST
//
// Summary
//      Encapsulates a candle stick
//

#ifndef CANDLESTICK_INCLUDED
#define CANDLESTICK_INCLUDED

#include<vector>
#include<string>
#include <boost/algorithm/string.hpp>
#include <assert.h>

class CandleStick  {
        double _open, _high, _close, _low;
        std::string _timestamp;
    public:
        CandleStick(const std::string& timestamp, 
                    double open, double high, double  close, double low):
                    _timestamp(timestamp),
                _open(open), _high(high),  _close(close), _low(low) {
        }

        std::string timestamp() const {
            return _timestamp;
        }

        double open() const {
            return _open;
        }

        double high() const {
            return _high;
        }

        double close() const {
            return _close;
        }

        double low() const {
            return _low;
        }

        std::string to_string() const {
            char buffer[1024];
            sprintf(buffer,"%0.5f %0.5f %0.5f %0.5f ",_open, _high, _close, _low);
            return buffer;
        }
};

#endif // CANDLESTICK_INCLUDED
