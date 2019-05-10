from  py_data_type import  *
import py_rdb

class pointer(object):
    def __init__(self,*args, **kwargs  ):
        self.__rawdata =  POINTER()
        self.__rawdata.pt_id =-1
        self.__name =None

        if  len(args) >0 :
            self.__name = args

        if  len(kwargs.keys())>0:
            try:
                self.__rawdata.stn_id = kwargs['stn_id']
                self.__rawdata.dev_id = kwargs['dev_id']
                self.__rawdata.data_type = kwargs['data_type']
                self.__rawdata.src_id = kwargs['update_src']
                self.__rawdata.pt_id = kwargs['pt_id']
            except Exception as  err:
                print( err)

    @property
    def rawdata(self):
        if self.__rawdata.pt_id >0  and  self.__rawdata.pt_id < 256 :
            return self.__rawdata
        if self.__name[0] !=None:
            print self.__name[0]
            err = py_rdb.GetPtIdByNameStr(self.__name[0],self.__rawdata )
            if not err:
                return self.__rawdata
            else:
                return None
        return  None

    @property
    def name(self):
        if self.__name !=None and len(self.__name)>0:
            return  self.__name
        elif self.__rawdata != None:
            r=py_rdb.GetEntryNameById(self.__rawdata)
            return  r[1] if r[0!=-1] else None
        else:
            raise ValueError('Please set pointer data as first!')
            
    @name.setter
    def name (self,value):
        if not isinstance(value, str):
            raise ValueError('score must be an string!')
        self.__name = value
        
    


# This file is compatible with both classic and new-style classes.



class hdb(object):
    def __init__(self,**argc):
        py_rdb._connect_H9000_EMS()
        pass
    def readpointer(self,pot):
        if not isinstance(pot,pointer):
            print ("type is error !")

        return py_rdb._readPointer(pot.rawdata)

    def readFloatvalByname(self,ptname):
        if not isinstance(ptname,str):
            print ("type is error !" )
            return None
        err,s =   py_rdb._PyReadFloatValByName(ptname )
        if not err:
            return s
        else:
            return None

    def brdcstPtValue(self,pt,val):
        if  not isinstance(val,int):
            print ("type is error !" )
            return None

        if  isinstance(pt,str):
            err = py_rdb.Broadcast_ana_lname(pt,val)
            if not err:
                return True
            else:
                return False
        elif isinstance(pt,pointer):
            rd = pt.rawdata
            if  rd !=None:
                err = py_rdb.Broadcast_ana(rd, val)
                if not err:
                    return True
                else:
                    return False
            return  False
        pass

    def brdcstPtStatus(self,pt,status):
        if  not isinstance(status,int):
            print ("type is error !" )
            return None

        if  isinstance(pt,str):
            err = py_rdb.Broadcast_sw_lname(pt,status)
            if not err:
                return True
            else:
                return False
        elif isinstance(pt,pointer):
            err = py_rdb.Broadcast_sw(pt.rawdata, status)
            if not err:
                return True
            else:
                return False

        pass