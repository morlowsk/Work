import optparse, cx_Oracle
from credentials import *

def main():
    p = optparse.OptionParser()
    p.add_option('--table', '-t', help='delete from tables')
    '''gets arguments from command line to parse them'''
    options, arguments = p.parse_args()
    
    if options.table:
        cursor = con.cursor()
        try:
            cursor.execute("select * from gkumar7.%s" % options.table)
            description = cursor.description
            '''doesn't allow admin to update derived attributes'''
            if options.table == 'orders':
                del description[-1]
                del description[-1]
            if options.table == 'order_details':
                del description[-1]
            attributes = [i[0].lower() for i in description]
            print "Columns to choose from %s\n" % ', '.join(attributes)
            while True:
                column = raw_input("Please enter the column to delete from: ")
                if column.lower() in attributes:
                    break
                else:
                    print "Column %s doesn't exist, try again:" % column

            value = raw_input("Please enter the value of the column: ")
            query = ("delete from gkumar7.%s where %s=:val") % (options.table, column)
            try:
                cursor.prepare(query)
                cursor.execute(None, {'val':value})
                con.commit()
            except Exception, e:
                print "Value %s doesn't exist" % value + str(e) + '\n'

        except Exception, e:
            print "Table %s doesn't exist" % options.table + '\n' + str(e)
    else:
        p.print_help()
if __name__ == '__main__':
    main()


 
