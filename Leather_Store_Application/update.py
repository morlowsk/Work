import optparse, cx_Oracle
from credentials import *
from collections import defaultdict

def main():
    p = optparse.OptionParser()
    p.add_option('--table', '-t', help='insert into tables')
    
    '''gets arguments from command line to parse them'''`
    options, arguments = p.parse_args()

    if options.table:
       cursor = con.cursor()
       try:
           cursor.execute("select * from gkumar7.%s" % options.table)
           description = cursor.description
           '''doesn't allow admin to update derived attribute columns'''
           if options.table == 'orders':
             del description[-1]
             del description[-1]
           if options.table == 'order_details':
             del description[-1] #update not allowed on derived attribute num_prod_ordered or price
           attributes = [i[0].lower() for i in description]
           '''denotes required attributes'''
           null_cons = ['(*)' if i[6] == 0 else '' for i in description]
           for_print = zip(attributes, null_cons)
           list_of_columns = []
           print "Required columns are denoted with a (*)"
           print "Columns to chose from: %s\n" % ', '.join([tpl[0] + " " + tpl[1] for tpl in for_print])
           while True:
               columns = raw_input("Please enter the columns to update, separated by commas: ")
               list_of_columns = columns.split(",")
               for column in list_of_columns:
                   if column.lower() not in attributes:
                       print "Column %s doesn't exist, try again:" % column
                       break
               else:
                   break #break out of while, all columns are in attributes
           conditional_column = ""
           while True:
               conditional_column = raw_input("Please enter the column on which these columns are condtional on: ")
               if conditional_column.lower() in attributes:
                   break
               else:
                   print "Column %s doesn't exist, try again:" % column
           list_of_columns.append(conditional_column)
           values = raw_input("Please enter the values for each column to be updated, separated by commas: ")
           list_of_values = values.split(",")
           conditional_value = raw_input("Please enter the value of column on which this is all conditional: ")
           i = 0
           update_query = "update gkumar7.%s set " % options.table + " ".join((str(list_of_columns[i])+"=:val" + str(i)+ ", " for i in range(len(list_of_values) )))
           '''gets rid of last comma''' 
           query1 = update_query[:-2]
           query2 = " where %s=:val%s" % (conditional_column, str(i+1))
           '''completes update query'''
           query = query1 + query2
           list_of_values.append(conditional_value) 
           valuedict = defaultdict(str)
           for j, value in enumerate(list_of_values): #last column is the one that every other column is conditional on
               valuedict['val' + str(j)] = value

           try:
               cursor.prepare(query)    
               cursor.execute(None, valuedict)
               con.commit()
           except Exception, e:
               print 'Error with values entered\n' + str(e) + '\n'         
       
       except Exception, e:
           print 'Table %s doesn\'t exist' % options.table + '\n' + str(e) + '\n'
    else:
        p.print_help()
if __name__ == '__main__':
    main()
