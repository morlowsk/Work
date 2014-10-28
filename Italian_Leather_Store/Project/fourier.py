import cx_Oracle
con = cx_Oracle.connect('morlowsk', 'weaselthorptlahuac1', cx_Oracle.makedsn('fourier.cs.iit.edu', 1521, 'orcl'))
cursor = con.cursor()
cursor.execute("select * from gkumar7.categories") #keep this as gkumar7

print [i[0] for i in cursor.description]
for col in cursor:
  print col
