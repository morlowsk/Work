import MySQLdb as db
con = db.connect('108.216.119.5', 'michael', 'michael', 'cs425_project')
cur = con.cursor()
cur.execute("select * from categories")

rows = cur.fetchall()

print [i[0] for i in cur.description]
for row in rows:
  print row
