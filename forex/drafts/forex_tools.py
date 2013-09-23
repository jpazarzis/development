#!/usr/bin/python

from tools import dbtools

sql_select_instruments = 'select instrument_id, name from instruments'
sql_create_instrument = "CALL create_instrument('{0}')"


name_to_id_map = None

def get_id_from_name(instruement_name):
    '''
    returns the internal id for the specificied instruement name
    as it comes from the data base. If the name does not already 
    exist in the data base then it is created.
    '''
    global name_to_id_map
    name = instruement
    if name_to_id_map is None:
        name_to_id_map = {}
        dbtools.db_db = 'forex_db'
        for row in dbtools.execute_query(sql_select_instruments):
            name_to_id_map[row.name] = row.instrument_id

    if name not in name_to_id_map:
        for row in dbtools.execute_query(sql_create_instrument.format(name)):
            name_to_id_map[name] = row.new_id
    
    return name_to_id_map[name]



if __name__ == '__main__':
    print 'testing forex_tools'
    i = get_id_from_name('test')
    print name_to_id_map



        

