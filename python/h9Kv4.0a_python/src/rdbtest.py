import py_rdb as rdb 
import pyh9ktype
r= 333
f=66.66
rdb._connect_H9000_EMS()
#r,f  = rdb._PyReadFloatValByName('DDJ.DDJ_F1.ANA.AI1_02')





dbname =  rdb.GetDBLongname()
print dbname 


num =  rdb.GetStationNum()
print num 
pt1 = pyh9ktype.pointer(stn_id =1,dev_id=1,data_type =1,update_src =204,pt_id=1)
name = pt1.name
pt1.name = '1.1.1.1'
pt2 = pyh9ktype.pointer('DDJ.DDJ_F1.ANA.AI1_02')
db = pyh9ktype.hdb()
import time
for i in range(1000):
    db.brdcstPtValue('DDJ.DDJ_F1.ANA.AI1_02',4321234)
    db.brdcstPtValue(pt1,12345)
    #name =db.readFloatvalByname('GH1U1.ZC01.ANA.Z02_CJQ1_012')
    db.brdcstPtStatus('DDJ.DDJ_F1.INT.SOE1_02',1)
    pt3= pyh9ktype.pointer('DDJ.DDJ_F1.INT.SOE1_02')
    db.brdcstPtStatus(pt3,0)
    db.brdcstPtStatus(pt3,1)
    time.sleep(1)





def register(f):
    print 'before decorate'
    r = [] 
    def fun(*args ):
        r.append(args[0])
        
        
        print (args ,  r)
        f(args)
        print('f xxxx close ')
    return  fun 
    print ('end decorate')

    
    

def decorated(*args):
    print ('in decorated')
    


decorated = register(decorated)

decorated (1)

