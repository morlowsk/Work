# Forum
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
from __future__ import print_function
import shlex
from User_Class import *
import sys
user=User_Class()

class Forum_Class:

    def crthread(self,con,cursor,tname,ruID):
        cursor.execute("select max(thid)+1 from Threadz;")
        nextID=cursor.fetchone()[0]
        if(nextID==None):
            nextID=0
        
        cursor.execute("insert into Threadz values({},'{}');".format(
            nextID,tname))
        
        cursor.execute("update RegUser set rewardpnts=rewardpnts+1,totalpnts=totalpnts+1 "
            "where ruId={}".format(ruID))
        con.commit()
        
        user.getReward(con,cursor,ruID)
        
        return nextID
    
    def crcomment(self,con,cursor,thname,contn,ruID):
        cursor.execute("select thid from Threadz where thd_name='{}'".format(thname))
        thrid=cursor.fetchone()
       
        if(thrid==None):
            print("No such thread")
            return

        thrid=thrid[0]
        cursor.execute("select max(comid)+1 from Comments")
        comtID=cursor.fetchone()[0]

        if(comtID==None):
            comtID=0

        cursor.execute("insert into Comments values({},'{}',0,{});".format(comtID,contn,thrid))
        cursor.execute("update RegUser set rewardpnts=rewardpnts+1,totalpnts=totalpnts+1 "
            "where ruId={}".format(ruID))
        con.commit()
        user.getReward(con,cursor,ruID)

        return comtID

    def viewthreads(self,con,cursor):
        cursor.execute("select thd_name from Threadz order by thid")
        allthreds=cursor.fetchall()
        for i in range (0,len(allthreds)):
            print("{}".format(allthreds[i][0]))
        
    
    def viewcomments(self,con,cursor,thredname,choice):

        cursor.execute("select thid from Threadz where thd_name='{}'".format(thredname))
        thredid=cursor.fetchone()

        if(thredid==None):
                print("No such thread")
                return
        
        thredid=thredid[0]

        if(choice=='all'):
            cursor.execute("select content,likes,comid from Comments where thread_id={} order by comid DESC".format(thredid))
            allcoms=cursor.fetchall()
            for i in range (0,len(allcoms)):
                print("{}> {}; {} likes".format(allcoms[i][2],allcoms[i][0],allcoms[i][1]))

        elif(choice=='recent'):
            cursor.execute("select content,likes,comid from Comments where thread_id={} order by comid DESC".format(thredid))
            allcoms=cursor.fetchall()
            
            if(len(allcoms)<10):
                thing=len(allcoms)
            else:
                thing=10

            for i in range (0,thing):
                print("{}> {}; {} likes".format(allcoms[i][2],allcoms[i][0],allcoms[i][1]))

        else:
            print("Those words were not that hard to type.")


    def viewthreadcoms(self,con,cursor):

        cursor.execute("select max(thid) from Threadz;")
        maxid=cursor.fetchone()[0]

        for i in range (0,maxid+1):
            cursor.execute("select thd_name from Threadz where thid={}".format(i))
            threadname=cursor.fetchone()[0]

            print("\n{}:".format(threadname))
            cursor.execute("select content,likes,comid from Comments where thread_id={} order by comid DESC".format(i))
            allcoms=cursor.fetchall()
            
            cursor.execute("select count(content) from Comments where thread_id={};".format(i))
            maxcom=cursor.fetchone()[0]

            if(maxcom<10):
                for x in range (0,maxcom): # Max com is incase we have les than 10 comments, 
                                           # we dont go out of tuple bounds
                    print("{}> {} ; {} likes".format(allcoms[x][2],allcoms[x][0],allcoms[x][1])) # In other words, 
                                                                   # display available comments with max of 10
            else:        
                for y in range (0,10): 
                    print("{}> {}; {} likes".format(allcoms[y][2],allcoms[y][0],allcoms[y][1]))

    def like(self,con,cursor):
        comid=raw_input("Which comment? ")

        cursor.execute("update Comments set likes=likes+1 where "
            "comid={}".format(comid))
        con.commit()
