#include	<math.h>


#include  "../inc/py_rdb.h"

#pragma	comment(lib, "advapi32.lib ")
#pragma	comment(lib, "/home/ems/h9000.xld/lib/xml2.lib")


static int  _init_h9000_env();
static short	cur_rec_pt = 0 ;

int _connect_H9000_EMS()
{
	if (_init_h9000_env() == -1)
		exit(-1);
	return 0;
}



int _disconnect_H9000_EMS()
{
	if (-1 == UnMountDB())
	{
		printf("\nFail to disconnect to RDB!");
		return(-1);
	}
	return  0; 
}

int _readPointer(POINTER *p)
{
	printf("Read a point stn_id=%d,dev_id=%d", p->stn_id, p->dev_id); 
	
	return  0; 
}

int	_PyReadLogicNamelByID( POINTER *pt,char *ptNameStr )
{
	char buf[128]; 

	printf("Read a point stn_id=%d,dev_id=%d,pt_id=%d", pt->stn_id, pt->dev_id, pt->pt_id);
	if (-1 == GetEntryNameById(pt, buf))
	{
		printf("\nNo this point in database:(%s)", ptNameStr);
	}
	
	strcpy(ptNameStr, buf); 
	printf("return val=%s", ptNameStr);
	return 0 ; 
}

int	_PyReadIntValByName(char *ptNameStr, int *value)
{
	DMS_COMMON common_all; 


	if (-1 == ReadEntryByNameStr(ptNameStr, &common_all))
	{
		printf("\nNo this point in database:(%s)",ptNameStr);
	}
	*value = common_all.var.anlg.value.data;

	return  0; 
}

int	_PyReadFloatValByName(char *ptNameStr, float *value)
{
	DMS_COMMON common_all;

	if (-1 == ReadEntryByNameStr(ptNameStr, &common_all))
	{
		printf("\nNo this point in database:(%s)", ptNameStr);
	}
	*value = common_all.var.anlg.fValue;

	return  0;
}
void Broadcast_ana(POINTER *pt, int value)
{
	LAN_HEAD		  lanhead;
	ANA_MSG  		  msg;
	struct tm       *datetime;
	time_t   t;
	t = time(NULL);
	datetime = localtime(&t);

	msg.type_id = DPS_ANALOG;
	msg.stn_id = pt->stn_id;
	msg.dev_id = pt->dev_id;
	msg.data_type = pt->data_type;
	msg.number = 1;
	msg.point_id = pt->pt_id;

	msg.status = 0;
	memcpy(msg.data, &value, 4);
	GetLocalHostId(&msg.host_id);

	t = time(NULL);
	datetime =	localtime(&t);
	msg.sec =	datetime->tm_sec;
	msg.min =	datetime->tm_min;
	msg.hour =	datetime->tm_hour;
	msg.day =	datetime->tm_mday;
	msg.month = datetime->tm_mon + 1;
	msg.year =	datetime->tm_year + 1900;

	lanhead.length = ANAMSG_SIZE;
	lanhead.dp_type = BJ_DB_TIME;
	lanhead.dest_id = 0;
	GetLocalHostId(&lanhead.src_id);
	lanhead.dest_stn = msg.stn_id;
	lanhead.packet_num = 0;
	lan_out((char *)&msg, lanhead, (unsigned char)0);
	return;

}




void Broadcast_sw(POINTER *pt, short stauts  )
{
	LAN_HEAD		  lanhead;
	SYS_MSG  		  msg;
	struct tm       *datetime;
	time_t   t;

 
	t = time(NULL); 
	datetime = localtime(&t);
	msg.type_id = DPS_IND_ALM;
	msg.stn_id = pt->stn_id;
	msg.dev_id = pt->dev_id;
	msg.data_type = pt->data_type;
	msg.number = 1;
	msg.point_id = pt->pt_id;
	msg.status = stauts + 256;  //�㲥ǰ+256
	GetLocalHostId(&msg.host_id);

	msg.ms = 0;
	msg.sec = datetime->tm_sec;
	msg.min = datetime->tm_min;
	msg.hour = datetime->tm_hour; 
	
	msg.day = datetime->tm_mday;
	msg.month = datetime->tm_mon + 1;
	msg.year = datetime->tm_year + 1900;

	lanhead.length = SYSMSG_SIZE;
	lanhead.dp_type = BJ_DB_TIME;
	lanhead.dest_id = 0;
	GetLocalHostId(&lanhead.src_id);
	lanhead.dest_stn = msg.stn_id;
	lanhead.packet_num = 0;
	lan_out((char *)&msg, lanhead, (unsigned char)0);
}

int  Broadcast_ana_lname(char * ptname, int value)
{
	POINTER pt; 
	if (-1 != GetPtIdByNameStr(ptname, &pt))
	{
		Broadcast_ana(&pt, value);
		return  0; 
	}
	return -1; 

}


int Broadcast_sw_lname(char * ptname, short status)
{
	POINTER pt;
	if (-1 != GetPtIdByNameStr(ptname, &pt))
	{
		Broadcast_sw(&pt, status);
		return  0;
	}
	return -1;

}

static int  _init_h9000_env()
{
	if (-1 == MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		return (-1);
	}

	if (lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		return (-1);
	}

	shm_rec_addr = (SHM_RECORD*)LinktoShareMem(SHM_REC_KEY_CHAR);
	cur_rec_pt = shm_rec_addr->head.header_ptr;
	if (shm_rec_addr == (SHM_RECORD*)-1)
	{

#ifdef WIN32
		printf("\n Fail to get share memory shm_rec_addr: errno=%d\n", GetLastError());
#else

		printf("\n Fail to get share memory shm_rec_addr: errno=%d\n", errno);
#endif
		exit(-1);
	}
	printf("\n pyH9000 init environment OK!\n");


}




//"GH1U1.ZC01.ANA.Z02_CJQ1_012"
int	main(int argc, char *argv[])	
{
 
	DATE_TIME	time = { 0 };
	int ivalue; 
	char buff[128]; 
	float fvalue =9.9;
	POINTER pt = {1,1,1,1,33};
    
	if (_init_h9000_env() == -1)
		exit(-1); 

	GetSysTime(&time);
	
	Broadcast_sw_lname("DDJ.DDJ_F1.POL.DI1_02", 0); 
	Broadcast_sw_lname("DDJ.DDJ_F1.POL.DI1_02", 1);
	Broadcast_ana_lname("DDJ.DDJ_F1.ANA.AI1_02",1234); 

	int r = GetPtIdByNameStr("DDJ.DDJ_F1.INT.SOE1_02", &pt);
	Broadcast_sw(&pt, 1);

	SetAnaPtfValueByNameStr("GH1U1.ZC01.ANA.Z02_CJQ1_012", fvalue, time);
	if (-1 == _PyReadFloatValByName("GH1U1.ZC01.ANA.Z02_CJQ1_012", &fvalue))
	{
		printf("\nfvalue error \n");


	}
	printf("\nsuccessfully fvalue = %f" ,fvalue);
	if (-1 == _PyReadIntValByName("GH1U1.ZC01.ANA.Z02_CJQ1_012", &ivalue))
	{
		printf("\nivalue error \n");
	}
	printf("\nsuccessfully and ivalue = %d",ivalue);
	_disconnect_H9000_EMS();
}


