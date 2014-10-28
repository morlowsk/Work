Application Detailed Instructions

Run application by typing in "python shell.py"
Do not use python 3!
Version we recommend: python 2.7.3

Application requires that you have cx_Oracle installed.
You may download it here: http://cx-oracle.sourceforge.net/
A nice tutorial on how to install it: http://bayo.opadeyi.net/2011/07/setting-up-cxoracle-on-ubuntu-1104.html

Application requires an admin username and password:
Username: admin@aol.com
Default Password: password

Command Details:

passwd: change the admin password
help: short list of commands available

report: 
  -t: specify the table which you would like to view
      
      if "names" is specified, the application will provide the names
      of all the tables in the database

  -f: specify the file you would like to write the information to
  -a: view table information based on advanced boolean expression
      specify keyword "advanced" after -a 

  Usage Example: report -t categories -f cat_report.txt -a advanced
      Application will prompt you with a series of questions for 
      boolean expressions which you would like to use on the categories
      table; output will be in the cat_report.txt file located in your
      present working directory

insert:
  -t: specify the table which you would like to insert data in 
  
  Usage Example: insert -t products
     Application will prompt you for a series of values for each column
     in the table; only columns denoted with a (*) are required

update:
  -t: specify the table where the data needs to be updated
  
  Usage Example: update -t products
     Application will prompt you for the following:
     columns which need to be updated;
     column on which above columns are conditional on (dependent on);
     updated values for the columns specified;
     current value for the conditional column

delete:
  -t: specify the table which you would like to delete data from
  
   Usage Example: delete -t orders
     Note this deletes an entire row in the specified table
     Application will prompt you for the column from which you want
     to delete and the current value of that column
  