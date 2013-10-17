#!/usr/bin/python

import json
 
json_input = '{ "one": 1, "two": { "list": [ {"item":"A"},{"item":"B"} ] } }'



def make_from_json(json_input):
    obj = type('Junk', (object,) ,json.loads(json_input))
    return obj
   
    


 
try:
    
    a = make_from_json(json_input)
    print a
    print a.one
    print a.two["list"]
    print dir(a)
 
except (ValueError, KeyError, TypeError):
    print "JSON format error"
