#!/usr/bin/python
def Calculator(str): 
  exec 'x='+str
  return x
    
# keep this function call here  
# to see how to enter arguments in Python scroll down
print Calculator(raw_input())  
