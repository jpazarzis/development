#!/usr/bin/python

def normalize_double(value,digits):
        '''
        Rounds the floating point value to the given precision.
        Returns normalized value of the double type. 

        The calculated StopLoss and TakeProfit values, as well 
        as open price of pending orders must be normalized with 
        a precision the value of which is stored in the 
        pre-defined variable of Digits.

        >>> normalize_double(0.123456789,5)
        0.12346

        '''

        return round(value,digits)

if __name__ == "__main__":
    import doctest
    doctest.testmod()
