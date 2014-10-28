import optparse, cx_Oracle
from credentials import *

'''Following used to retrive reports and print them as relations 
or place them in a separate file provided by user'''

def main():
  cursor = con.cursor()
  p = optparse.OptionParser()
  p.add_option('--table', '-t', help='report all data in table specified (default), specify "names" for names of all tables')#\nTables to choose from: %s' % str([col for col in cursor]))
  p.add_option('--filename', '-f', help='place report in a file; filename is provided by user')
  p.add_option('--advanced', '-a', help='type "advanced" after flag; advanced options; user is able to narrow down data returned')
  options, arguments = p.parse_args()
  
  if options.table == 'names':
    try:
      cursor.execute("select table_name from all_tables where owner = 'GKUMAR7'")
      print [i[0] for i in cursor.description]
      for col in cursor:
          print col[0]
    except Exception, e:
      print("Database execution error\n" + str(e) + '\n')
    
  elif options.table:
    
    try:
      cursor.execute("select * from gkumar7.%s" % options.table)
    except: print 'table %s not found' % options.table

    if options.advanced:

      print("Attributes to choose from: \n")
      print [i[0] for i in cursor.description] 
      attribute = raw_input("Enter an attribute name: ")
      print("Boolean operands to choose from: >, >=, <>, =, <, <=\n")
      
      operation = raw_input("Operand for " + attribute + ": ")
      value = raw_input("Check value: ")
      specifics = raw_input("Would you like to print all columns? (yes or no): ")
      if specifics == 'no':
        atts = raw_input("Enter attributes you would like in a comma separated list (att1, att2): ")
        try:
          cursor.prepare("select %s from gkumar7.%s where %s%s:val" % (atts, options.table, attribute, operation))
          cursor.execute(None, {'val':value})
        except Exception, e: print 'Illegal expression\n' + str(e) + '\n'
      else:
        try:
          cursor.prepare("select * from gkumar7.%s where %s%s:val" % (options.table, attribute, operation))
          cursor.execute(None, {'val':value})
        except Exception, e: print 'Illegal expression\n' + str(e) + '\n'

    if options.filename:
      f = open(options.filename, 'a')
      f.write(str([i[0] for i in cursor.description]) + '\n')
      for col in cursor:
        f.write(str(col) + '\n')

    else:
      print [i[0] for i in cursor.description]
      for col in cursor:
          print col
    
  else:
    p.print_help()

if __name__ == '__main__':
  main()
