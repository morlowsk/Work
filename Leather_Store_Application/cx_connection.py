'''Following program is just for your reference to see how the connection works'''

import cx_Oracle
from credentials import *

cursor = con.cursor()
cursor.execute("select * from gkumar7.products")

print [i[0] for i in cursor.description]
for col in cursor:
  print col
