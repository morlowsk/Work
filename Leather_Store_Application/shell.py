from __future__ import print_function

import readline, threading, sys, cx_Oracle, os, subprocess, getpass
from credentials import *
from adminpass import * 
class shell(object):
  
  '''Database Connection'''
  def __init__(self):
    self.con = con
    self.cursor = self.con.cursor()
    self.PROMPT = '> '
    threading.Thread(target=self.cli).start()
    self.username = 'admin@aol.com'
    self.password = password
    self.startprog = False
    #self.cli()

  '''Below are shell specific commands'''
  def builtin_cmd(self, cmd): #shell specific commands
    if cmd[0] == 'exit':
        sys.exit(0)
    elif cmd[0] == 'help': 
        #list the different options for the shell (exit, etc.)
        print("\nAllowed Commands:\n\nexit: exit shell\n\nreport: retrieve reports about the database\n\npasswd: change admin password\n\ninsert: insert a row of data into a table in the database\n\nupdate: update a row of data in the database\n\ndelete: delete a row of data in the database based on the value of a specified attribute in that row\n\nall linux shell commands will work as well\n")
    elif cmd[0] == 'passwd':
        passcheck = getpass.getpass("Current password: ")
        if passcheck == self.password:
          newpass = getpass.getpass("Enter new password: ")
          newpass2 = getpass.getpass("Reenter password: ")
          if newpass2 == newpass:
            f = open('adminpass.py', 'w')
            f.write("password = '" + newpass + "'")
            print("Password has successfully been changed")
          else:
            print("Passwords do not match")
        else:
          print("Authentication error")
    #elif cmd == 'clear':
     #   os.system("clear")

  '''Check to see if user entered linux shell command'''
  def linux_cmd(self, cmd):
    try:
      subprocess.call(cmd)
    except OSError:
      self.error(cmd)

  '''Below add functions for different aspects of database such as add product, list all products, etc'''
  def data_cmd(self, cmd): 
    if cmd[0] == 'report':
      os.system('python report.py %s' % ' '.join(cmd[1:]))
      #print('python report.py %s' % ' '.join(cmd[1:]))
    elif cmd[0] == 'insert':
      os.system('python insert.py %s' % ' '.join(cmd[1:]))
      #print('python insert.py %s' % ' '.join(cmd[1:]))
    elif cmd[0] == 'delete':
      os.system('python delete.py %s' % ' '.join(cmd[1:]))
      #print('python insert.py %s' % ' '.join(cmd[1:]))
    elif cmd[0] == 'update':
      os.system('python update.py %s' % ' '.join(cmd[1:]))
      #print('python insert.py %s' % ' '.join(cmd[1:]))
 
  '''Finally, if the user inputs any other command; an error will be shown.'''
  def error(self, cmd):
    print("Error, please type \'help\' to see a list of valid commands.")

  '''Do not modify interrupt()'''
  def interrupt(self):
    print() # Don't want to end up on the same line the user is typing on.
    print(self.PROMPT, readline.get_line_buffer(), sep='', end='')

  '''Below if user types a builtin_cmd or sql specific instruction, execute the correct function'''
  def cli(self): #cmd is passed as a list of arguments
    usercheck = raw_input("Username: ")
    passcheck = getpass.getpass("Password: ")
    if usercheck == self.username and passcheck == self.password:
      print("\nYou are logged in.")
      self.startprog = True
    else:
      print("\nIncorrect username or password")
      self.startprog = False
    while self.startprog:
        cli = str(raw_input(self.PROMPT)).split(' ')
        if cli[0] in ['exit', 'help', 'passwd']:
          self.builtin_cmd(cli)
        elif cli[0] in ['report']:
          self.data_cmd(cli)
        elif cli[0] in ['insert']:
          self.data_cmd(cli)
        elif cli[0] in ['delete']:
          self.data_cmd(cli)
        elif cli[0] in ['update']:
          self.data_cmd(cli)
        else:
          self.linux_cmd(cli)
          
if __name__ == '__main__':
    shell()

