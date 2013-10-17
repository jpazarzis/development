#!/usr/bin/python   

from ZODB.FileStorage import FileStorage
from ZODB.DB import DB
import transaction

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age


def create():
        storage = FileStorage('Data.fs')
        db = DB(storage)
        connection = db.open()
        root = connection.root()
        root ['p1'] = Person('john',49)
        root ['p2'] = Person('nick',35)
        transaction.commit()
        print root.keys()

def open_db():
        storage = FileStorage('Data.fs')
        db = DB(storage)
        connection = db.open()
        root = connection.root()

        for k in root:
            a = root[k]
            print a.name
   


if __name__ == '__main__':
    open_db()

