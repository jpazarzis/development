#!/usr/bin/python

''' Simple data base access tool'''


import MySQLdb as mdb
import atexit

db_connection = None
db_host="localhost"
db_user="john"
db_db="test"

def connect():
    global db_connection
    db_connection = mdb.connect(host=db_host,user=db_user,db=db_db)
    atexit.register(close_connection)


def connect_if_needed():
    ''' connects if there is no connection so far'''
    global db_connection
    if not db_connection:
        connect()

def close_connection():
    ''' closes the connection'''
    global db_connection
    if db_connection:
        db_connection.close()
        db_connection=None


def execute_query2(sql,*args):
        connect_if_needed()
        cur = db_connection.cursor(mdb.cursors.DictCursor)
        cur.execute(sql.format(*args))
        for i in range(cur.rowcount):
            yield cur.fetchone()


def execute_query(sql,*args):
        ''' executes a sql statement returning each row as an object
            with property names matching the columns requested in the sql '''
        connect_if_needed()
        cur = db_connection.cursor(mdb.cursors.DictCursor)
        cur.execute(sql.format(*args))
        for i in range(cur.rowcount):
            yield type('QueryResults', (object,), cur.fetchone())

if __name__ == '__main__':
            print 'testing race results'
            SQL_GET_RESULTS_FOR_RACE="select id, program_number, odds, finish_position, race_id from  race_starters where track_code = '{0}' and racing_date = '{1}' and race_number={2} and program_number != 'SCR'"
            for c in execute_query(SQL_GET_RESULTS_FOR_RACE, 'AQU','20100101',1):
                print c.odds, c.id, c.finish_position



