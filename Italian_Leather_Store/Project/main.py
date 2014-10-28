#!/usr/bin/python2
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
# -*- coding: utf-8 -*-

from __future__ import print_function
import MySQLdb as mdb
import shlex
from collections import deque
import sys
#from User_Class import * 
from Flow_Class import *

def work(con,cursor):
        flow.loop(con,cursor)

# Connects and then calls the work() function
def connect():
   
    if sys.platform == "win32":
        # Windows...
        host = '127.0.0.1'
    elif sys.platform == "linux" or sys.platform == "linux2":
        # linux
        host = 'localhost'
    try:
        con = None
        con = mdb.connect(host,'bot','clonepenguin','RVAJ');
        cursor = con.cursor()
        work(con,cursor)
        con.commit()
    except mdb.Error, e:
        print("Error {}: {}".format(e.args[0],e.args[1]))
        sys.exit(1)

    finally:    
        if con:
            con.close()

if __name__ == '__main__':
    # Class Intializations
    #user=User_Class()
    flow=Flow_Class()
    connect()
