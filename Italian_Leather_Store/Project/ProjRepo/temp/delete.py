import optparse, cx_Oracle
from credentials import *

def main():
    p = optparse.OptionParser()
    p.add_option('--table', '-t', help='delete from tables')

    options, arguments = p.parse_args()
    
    if options.table:
        cursor = con.cursor()
        try:
            cursor.execute("select * from gkumar7.%s" % options.table)
            attributes = [i[0] for i in cursor.description]
            while True:
                column = raw_input("Please enter the column to delete from: ")
                if column in attributes:
                    break
                else:
                    print "Column %s doesn't exist, try again:" % column

            value = raw_input("Please enter the value of the column: ")
            query = ("delete from gkumar7.%s where %s=:val") % (options.table, column)
            try:
                cursor.prepare(query)
                cursor.execute(None, {'val':value})
  
            except:
                print "Value %s doesn't exist" % value

        except:
            print "Table %s doesn't exist" % options.table

if __name__ == '__main__':
  main()
 
