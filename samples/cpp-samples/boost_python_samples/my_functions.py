number_of_parameters = 2

class Parameter:
    def __init__(self, name, min_value, max_value, decimal_points):
        self.name = name
        self.min_value = min_value
        self.max_value = max_value
        self.decimal_points = decimal_points


parameters = ( Parameter('minute',10,60,1),
               Parameter('expiration',15,160,1)
        )        

def process_tick(year,month,day, hour, minute,second, bid, ask, minute_to_trade, expiration_minute, triggering_delta):
    return 1111.0 
