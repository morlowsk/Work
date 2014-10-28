import optparse, cx_Oracle
from credentials import *
from collections import defaultdict
import time

def update_quantity_instock(adv_id, val_ordered):
    cursor = con.cursor()
    select_q = "select quantity_instock from gkumar7.prod_advanced where adv_id = %s" % adv_id
    try:
      cursor.execute(select_q)
      for col in cursor:
        quantity_instock = col[0]
      #print quantity_instock
    except Exception, e:
      print "Error: \n" + str(e) + '\n'
    '''Following updates the quantity instock when order_details is inserted, see line 78'''
    update_q = "update gkumar7.prod_advanced set quantity_instock = %s where adv_id = %s" % (str(int(quantity_instock) - int(val_ordered)), adv_id)
    try:
      cursor.execute(update_q)
      con.commit()
    except Exception, e:
      print "Error: \n" + str(e) + '\n'    
    
'''Following used to insert values into specified tables among which are: manufacturers, categories, products, orders, order_details, '''

def main():
    p = optparse.OptionParser()
    p.add_option('--table', '-t', help='insert into tables')
    
    '''gets arguments from command line to parse them'''
    options, arguments = p.parse_args()
    
    if options.table:
       cursor = con.cursor()
       try:
         cursor.execute("select * from gkumar7.%s" % options.table)
       except: print "table %s doesn't exist" % options.table
      
       try:
         attributes = [i[0] for i in cursor.description]
         null_cons = ['(*)' if i[6] == 0 else ''  for i in cursor.description]
         vals = [None] * len(attributes)
         if options.table == 'orders': '''Gets time from user to input into orders'''
           vals[2] = time.strftime("%Y/%m/%d %H:%M:%S")
           
         print "Required fields are denoted with a (*)"

         '''makes sure can't admin can't update derived values'''
         
         if options.table == 'orders':
           del vals[-1]
           del vals[-1]
           del attributes[-1]
           del attributes[-1]

         if options.table == 'order_details':
           del vals[-1] #remove virtual column
           del attributes[-1]

         if options.table == 'orders':
           for index, att in enumerate(attributes):
             if index != 2:
               vals[index] = raw_input("Enter value for %s %s: " % (att, null_cons[index]))

         else:
           for index, att in enumerate(attributes):
             vals[index] = raw_input("Enter value for %s %s: " % (att, null_cons[index]))
         '''Following code creates a prepared statement of query to be executed, values change based on what table you're inserting into.'''   
         insert_q = "insert into gkumar7.%s (%s) values (" % (options.table, ', '.join(attributes)) + ", ".join([":val" + str(i) for i in range(len(vals))]) + ")"
         cursor.prepare(insert_q)
         valuedict = defaultdict(str)
         for i, value in enumerate(vals):
           valuedict['val' + str(i)] = vals[i]
         cursor.execute(None, valuedict)
         con.commit()

       except Exception, e:
         print 'Error with values entered\n' + str(e) + '\n'

       
       if options.table == 'order_details':
         update_quantity_instock(vals[2], vals[3])

    else:
        p.print_help() 
     
if __name__ == '__main__':
    main()
