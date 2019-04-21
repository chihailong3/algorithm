/*********************************************************************************************
	文件名称：	zconfigfileinfo.cxx
	内    容：	配置文件信息和条件类(ZConfigFileInfo)实现
	功    能：	读取配置信息，读取初始化对象信息，处理事项保存条件判断
	主要函数：	1. GetInitObjectBuf()：公共成员函数
                功能：获得初始化历史事项对应的遥信对象
				2. GetInitEntTypeStr()：公共成员函数
                功能：获得初始化历史事项类型条件字符串
				3. IsMeetCondition()：公共成员函数
                功能：判断事项是否满足保存条件
				4. GetConfigFileInfoObj()：静态成员函数
                功能：获得配置类对象
				5. ReadConfigFile()：私有成员函数
                功能：读取配置文件信息
				6. ReadInitObjectInfo()：私有成员函数
                功能：读取初始化历史事项对应的遥信对象
				7. IsMeetEntType()：私有成员函数
                功能：判断事项类型是否满足保存条件
				8. IsMeetEntObject()：私有成员函数
                功能：判断事项对象是否满足保存条件
				9. GetRdbTableNameByObjectType()：私有成员函数
                功能：通过对象类型获得实时库表名
				10. IsProcModeMeetCondition()：私有成员函数
                功能：判断遥信对象报警处理方式是否满足保存条件
				11. GetEntProcModePara()：私有成员函数
                功能：获得事项处理方式参数
				12. SetReloadEntProcModePara()：公共成员函数
                功能：设置重新读取事项处理方式参数

	其它说明：	无
	修改日志：	无
*********************************************************************************************/

#include "zconfigfileinfo.h"
#include "selfini.h"
#include "dorbase.h"

#define Is_YX_Scada_Event(a)	(a >= ENTTYPE_STATUS && a <= ENTTYPE_YUGAOSOE)

//下面的遥信分、合报警与salarm的遥信分合报警应该一致
const int ciYxOffAlarm  = 11;		//遥信分报警
const int ciYxOnAlarm  = 12;		//遥信合报警

const int ciProcModeCoef = 1000;	//通过state_value2计算报警处理方式的系数

ZConfigFileInfo::ZConfigFileInfo()
{
	memset(m_acInitEntTypeStr, 0, sizeof(m_acInitEntTypeStr));
	m_bReloadProcModeParaF = true;
	m_iObjectTypeNum = 0;
	m_pObjectTypeBuf = NULL;
	m_iEventTypeNum = 0;
	m_pEventTypeBuf = NULL;
	m_iInitObjectNum = 0;
	m_pInitObjectBuf = NULL;
	m_iEventProcModeNum = 0;
	m_pEventProcModeBuf = NULL;

	ReadConfigFile();
	ReadInitObjectInfo();
}

ZConfigFileInfo::~ZConfigFileInfo()
{
	if(m_pObjectTypeBuf != NULL)
	{
		free(m_pObjectTypeBuf);
		m_pObjectTypeBuf = NULL;
	}
	if(m_pEventTypeBuf != NULL)
	{
		free(m_pEventTypeBuf);
		m_pEventTypeBuf = NULL;
	}
	if(m_pEventProcModeBuf != NULL)
	{
		free(m_pEventProcModeBuf);
		m_pEventProcModeBuf = NULL;
	}
}

/*************************************************
  函数名称:		ReadConfigFile()
  功    能:     读取配置文件信息
  被调函数:     1, 构造函数 
  调用函数:     1. CSelfIni::GetAppList()
				2. CSelfIni::GetFirstByNo()
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     void
  其它说明:     无
*************************************************/
void ZConfigFileInfo::ReadConfigFile()
{
	memset(m_acInitEntTypeStr, 0, sizeof(m_acInitEntTypeStr));
	m_iObjectTypeNum = 0;
	if(m_pObjectTypeBuf != NULL)
	{
		free(m_pObjectTypeBuf);
		m_pObjectTypeBuf = NULL;
	}
	m_iEventTypeNum = 0;
	if(m_pEventTypeBuf != NULL)
	{
		free(m_pEventTypeBuf);
		m_pEventTypeBuf = NULL;
	}

	int i = 0;
	char* pTmpStr = NULL;

	CSelfIni selfIniObj("uif/resource/alarmsrv_config.ini");
	selfIniObj.GetAppList("[初始化对象类型]");

	pTmpStr = selfIniObj.GetFirstByNo(i);
	while(pTmpStr != NULL)
	{
		m_pObjectTypeBuf = (int*)realloc(m_pObjectTypeBuf, sizeof(int)*(m_iObjectTypeNum+1));
		*(m_pObjectTypeBuf+m_iObjectTypeNum) = atoi(pTmpStr);

		m_iObjectTypeNum++;
		i++;
		pTmpStr = selfIniObj.GetFirstByNo(i);
	}

	selfIniObj.GetAppList("[保存事项类型]");
	i = 0;
	pTmpStr = selfIniObj.GetFirstByNo(i);
	while(pTmpStr != NULL)
	{
		m_pEventTypeBuf = (int*)realloc(m_pEventTypeBuf, sizeof(int)*(m_iEventTypeNum+1));
		*(m_pEventTypeBuf+m_iEventTypeNum) = atoi(pTmpStr);
		
		if(m_iEventTypeNum == 0)
		{
			sprintf(m_acInitEntTypeStr, "(%d", *(m_pEventTypeBuf+m_iEventTypeNum));
		}
		else
		{
			char acTmpStr[48];
			sprintf(acTmpStr, ",%d", *(m_pEventTypeBuf+m_iEventTypeNum));
			strcat(m_acInitEntTypeStr, acTmpStr);
		}
		m_iEventTypeNum++;
		i++;
		pTmpStr = selfIniObj.GetFirstByNo(i);
	}
	if(strlen(m_acInitEntTypeStr) > 0) 
	{
		strcat(m_acInitEntTypeStr, ")");
	}
	else
	{
		sprintf(m_acInitEntTypeStr, "(%d,%d)",
			//ENTTYPE_PROTSIGSOE,	//保护信号SOE
			//ENTTYPE_YUGAOSOE,	//预告信号SOE
			ENTTYPE_PROTSIG,	//保护信号变位
			ENTTYPE_YUGAO		//预告信号变位
			);
	}
}

/*************************************************
  函数名称:		ReadInitObjectInfo()
  功    能:     读取初始化历史事项对应的遥信对象
  被调函数:     1. 构造函数 
				2. GetInitObjectBuf()
  调用函数:     1. Rsqlclient::selectfrom()
				2. Rsqlclient::selectattr()
				3. Rsqlclient::where()
				4. Rsqlclient::select()
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     void
  其它说明:     无
*************************************************/
void ZConfigFileInfo::ReadInitObjectInfo()
{
	m_iInitObjectNum = 0;
	if(m_pInitObjectBuf != NULL)
	{
		free(m_pInitObjectBuf);
		m_pInitObjectBuf = NULL;
	}

	int i,j;
	char *pc = NULL;
	int iRecNum = 0;
	int iRetVal = 0;
	int iTmpType= 0;
	int iTmpVal	= 0;
	Rsqlclient rsql;
	int iProcType = 0;

	if(m_iObjectTypeNum == 0)
	{//读取保护类初始化对象
		rsql.selectfrom("protsig");
		rsql.selectattr("name");
		rsql.selectattr("entprocmode");
		rsql.selectattr("value");
//		rsql.where("entprocmode", DATATYPE_INT, &iProcType, ATTRCOND_NOTEQU);
		iRetVal = rsql.select("rsroot", "");
		if(iRetVal == RDB_OK)
		{
			iRecNum = rsql.getrcdcnt();
			if(iRecNum > 0)
			{
				for(i=0; i<iRecNum; i++)
				{
					pc = rsql.get_string(i,"name");
					iTmpType = (int)rsql.get_intval(i,"entprocmode");
					iTmpVal = (int)rsql.get_intval(i,"value");
					if(IsProcModeMeetCondition(iTmpType, iTmpVal))
					{
						m_pInitObjectBuf = (InitObjectInfoS*)realloc(m_pInitObjectBuf, sizeof(InitObjectInfoS)*(m_iInitObjectNum+1));
						strcpy((m_pInitObjectBuf+m_iInitObjectNum)->cObjCode, pc);
						(m_pInitObjectBuf+m_iInitObjectNum)->iEntMode = (unsigned char)iTmpType;
						(m_pInitObjectBuf+m_iInitObjectNum)->ucValue = (unsigned char)iTmpVal;
						m_iInitObjectNum++;
					}
				}
			}
		}
	}
	else
	{
		for(i=0; i<m_iObjectTypeNum; i++)
		{
			iRetVal = 0;
			iRecNum = 0;
			unsigned short	usObjType = *(m_pObjectTypeBuf+i);
			const char* pRdbTableName = GetRdbTableNameByObjectType(usObjType);
			rsql.ClearSQLCommand();
			rsql.selectfrom((char*)pRdbTableName);
			rsql.selectattr("name");
			rsql.selectattr("entprocmode");
			rsql.selectattr("value");
//			rsql.where("entprocmode", DATATYPE_INT, &iProcType, ATTRCOND_NOTEQU);
			rsql.where("baseobjtype", DATATYPE_USHORT, &usObjType);
			iRetVal = rsql.select("rsroot", "");
			if(iRetVal == RDB_OK)
			{
				iRecNum = rsql.getrcdcnt();
				if(iRecNum > 0)
				{
					for(j=0; j<iRecNum; j++)
					{
						pc = rsql.get_string(j,"name");
						iTmpType = (int)rsql.get_intval(j,"entprocmode");
						iTmpVal = (int)rsql.get_intval(j,"value");
						if(IsProcModeMeetCondition(iTmpType, iTmpVal))
						{
							m_pInitObjectBuf = (InitObjectInfoS*)realloc(m_pInitObjectBuf, sizeof(InitObjectInfoS)*(m_iInitObjectNum+1));
							strcpy((m_pInitObjectBuf+m_iInitObjectNum)->cObjCode, pc);
							(m_pInitObjectBuf+m_iInitObjectNum)->iEntMode = (unsigned char)iTmpType;
							(m_pInitObjectBuf+m_iInitObjectNum)->ucValue = (unsigned char)iTmpVal;
							m_iInitObjectNum++;
						}
					}
				}
			}
		}
	}
}

/*************************************************
  函数名称:		GetInitObjectBuf()
  功    能:     获得初始化历史事项对应的遥信对象
  被调函数:     1. InitHisEventsThread::run()
				2. IsMeetCondition()
  调用函数:     1. ReadInitObjectInfo()
  访问的表:		无
  修改的表:		无
  输入参数:     1. bool bReloadF : 是否重新读取初始化遥信对象
  输出参数:     1. int& iInitObjNum : 初始化遥信对象个数
  函数返回:     InitObjectInfoS* : 初始化遥信对象缓冲区
  其它说明:     无
*************************************************/
InitObjectInfoS* ZConfigFileInfo::GetInitObjectBuf(int& iInitObjNum, bool bReloadF)
{
	if(bReloadF)
	{
		ReadInitObjectInfo();
	}

	iInitObjNum = m_iInitObjectNum;

	return m_pInitObjectBuf;
}

/*************************************************
  函数名称:		GetInitEntTypeStr()
  功    能:     获得初始化历史事项类型条件字符串
  被调函数:     1. InitHisEventsThread::run()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     char* : 初始化历史事项类型条件字符串
  其它说明:     无
*************************************************/
char* ZConfigFileInfo::GetInitEntTypeStr()
{
	if(strlen(m_acInitEntTypeStr) <= 0)
	{
		return NULL;
	}

	return m_acInitEntTypeStr;
}

/*************************************************
  函数名称:		GetRdbTableNameByObjectType()
  功    能:     通过对象类型获得实时库表名
  被调函数:     1. ReadInitObjectInfo()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     1. int iObjType : 对象类型
  输出参数:     无
  函数返回:     const char* : 实时库表名
  其它说明:     无
*************************************************/
const char* ZConfigFileInfo::GetRdbTableNameByObjectType(int iObjType)
{
	const char* cYXRdbTableName[] = {"break","switch","protsig","status"};

	int index = 0;
	if(iObjType >= OBJTYPE_STATUS &&	iObjType < OBJTYPE_SWITCH)
	{
		index = 3;
	}
	else if(iObjType >= OBJTYPE_SWITCH &&	iObjType < OBJTYPE_PROTSIG)
	{
		index = 1;
	}
	else if(iObjType >= OBJTYPE_PROTSIG)
	{
		index = 2;
	}

	return cYXRdbTableName[index];
}

/*************************************************
  函数名称:		IsMeetCondition()
  功    能:     判断事项是否满足保存条件
  被调函数:     1. TreatEveThread::treatOneSysEvent()
  调用函数:     1. GetInitObjectBuf()
				2. IsMeetEntType()
				3. IsMeetEntObject()
  访问的表:		无
  修改的表:		无
  输入参数:     1. SYS_EVENT* pBuf : 系统事项缓冲区
  输出参数:     无
  函数返回:     bool : true,条件满足;false,条件不满足
  其它说明:     无
*************************************************/
bool ZConfigFileInfo::IsMeetCondition(SYS_EVENT* pBuf)
{
	if(pBuf == NULL) return false;

	int iInitObjNum;

	//加载遥信参数表，重读参数
	if(pBuf->event_sort == EVENT_OPERATION
		&& pBuf->event_type == OPTYPE_RDB_LOADTABLE)
	{//加载实时库
		if(strcmp(pBuf->member_name1, "break") == 0
			|| strcmp(pBuf->member_name1, "switch") == 0
			|| strcmp(pBuf->member_name1, "status") == 0
			|| strcmp(pBuf->member_name1, "protsig") == 0)
		{
			GetInitObjectBuf(iInitObjNum, true);
		}
	}

	if(!IsMeetEntType(pBuf->event_type)) return false;

	if(Is_YX_Scada_Event(pBuf->event_type) && !IsMeetEntObject(pBuf)) return false;

	return true;
}

/*************************************************
  函数名称:		IsMeetEntType()
  功    能:     判断事项类型是否满足保存条件
  被调函数:     1. IsMeetCondition()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     int iEntType : 事项类型
  输出参数:     无
  函数返回:     bool : true,条件满足;false,条件不满足
  其它说明:     无
*************************************************/
bool ZConfigFileInfo::IsMeetEntType(int iEntType)
{
	if(m_iEventTypeNum <= 0)
	{
		if(iEntType != ENTTYPE_PROTSIG && iEntType != ENTTYPE_YUGAO) return false;
		else return true;
	}
	else
	{
		for(int i=0; i<m_iEventTypeNum; i++)
		{
			if(iEntType == *(m_pEventTypeBuf+i))
			{
				return true;
			}
		}
		return false;
	}
}

/*************************************************
  函数名称:		IsMeetEntObject()
  功    能:     判断事项对象是否满足保存条件
  被调函数:     1. IsMeetCondition()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     1. SYS_EVENT* pBuf : 系统事项缓冲区
  输出参数:     无
  函数返回:     bool : true,条件满足;false,条件不满足
  其它说明:     无
*************************************************/
bool ZConfigFileInfo::IsMeetEntObject(SYS_EVENT* pBuf)
{
	int iProcMode = pBuf->state_value2%ciProcModeCoef;
	int iState = pBuf->state_value;

	if(IsProcModeMeetCondition(iProcMode, iState))
	{
		return true;
	}
	return false;
}

/*************************************************
  函数名称:		IsProcModeMeetCondition()
  功    能:     判断遥信对象报警处理方式是否满足保存条件
  被调函数:     1. ReadInitObjectInfo()
				2. IsMeetEntObject()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     1. const int & procmode : 报警处理方式
				1. const int & state : 对象状态
  输出参数:     无
  函数返回:     bool : true,条件满足;false,条件不满足
  其它说明:     无
*************************************************/
bool ZConfigFileInfo::IsProcModeMeetCondition(const int & procmode, const int & state)
{
	GetEntProcModePara();

	for(int i=0; i<m_iEventProcModeNum; i++)
	{
		if(procmode == (m_pEventProcModeBuf+i)->index)
		{
			if((m_pEventProcModeBuf+i)->alarm_mode == ciYxOffAlarm
				&& state == 0)
			{//分报警，状态为0，返回true
				return true;
			}
			else if((m_pEventProcModeBuf+i)->alarm_mode == ciYxOnAlarm
				&& state == 1)
			{//合报警，状态为1，返回true
				return true;
			}
			break;
		}
	}
	return false;
}

/*************************************************
  函数名称:		GetEntProcModePara()
  功    能:     获得事项处理方式参数
  被调函数:     1. IsProcModeMeetCondition()
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     无
  其它说明:     无
*************************************************/
void ZConfigFileInfo::GetEntProcModePara()
{
	if(m_pEventProcModeBuf != NULL && !m_bReloadProcModeParaF) return;

	m_bReloadProcModeParaF = false;
	m_iEventProcModeNum = 0;
	if(m_pEventProcModeBuf != NULL)
	{
		free(m_pEventProcModeBuf);
		m_pEventProcModeBuf = NULL;
	}

	int i;
	int retVal;
	char *pc = NULL;
	int tmpInt;
	
	Rsqlclient rsql;
	rsql.selectfrom("eprocmode");
	rsql.selectattr("no");
	rsql.selectattr("desc");
	rsql.selectattr("obj_type");
	rsql.selectattr("alm_mode");
	rsql.selectattr("ui_mode");
	rsql.selectattr("delay_sec");
	rsql.selectattr("over1");
	retVal = rsql.select("rsroot", "");
	if(retVal == RDB_OK)
	{
		m_iEventProcModeNum = rsql.getrcdcnt();
		if(m_iEventProcModeNum > 0)
		{
			m_pEventProcModeBuf = (EntProcModeData*)malloc(sizeof(EntProcModeData)*m_iEventProcModeNum);
			memset(m_pEventProcModeBuf, 0, sizeof(EntProcModeData)*m_iEventProcModeNum);
			for(i=0; i<m_iEventProcModeNum; i++)
			{
				tmpInt = (int)rsql.get_intval(i,"no");
				(m_pEventProcModeBuf+i)->index = tmpInt;
				pc = rsql.get_string(i,"desc");
				strcpy((m_pEventProcModeBuf+i)->desc, pc);
				tmpInt = (int)rsql.get_intval(i,"obj_type");
				(m_pEventProcModeBuf+i)->obj_type = tmpInt;
				tmpInt = (int)rsql.get_intval(i,"alm_mode");
				(m_pEventProcModeBuf+i)->alarm_mode = tmpInt;
				tmpInt = (int)rsql.get_intval(i,"ui_mode");
				(m_pEventProcModeBuf+i)->proc_mode = tmpInt;
				tmpInt = (int)rsql.get_intval(i,"delay_sec");
				(m_pEventProcModeBuf+i)->time_delay = tmpInt;
				tmpInt = (int)rsql.get_intval(i,"over1");
				(m_pEventProcModeBuf+i)->over_limit_mode = tmpInt;
			}
		}
	}
	else
	{
		m_iEventProcModeNum = 0;
		printf("Rsqlclient(eprocmode) fail\n");
	}
}

/*************************************************
  函数名称:		GetConfigFileInfoObj()
  功    能:     获得配置类对象
  被调函数:     1. TreatEveThread::treatOneSysEvent()
				2. InitHisEventsThread::run()
  调用函数:     1. ZConfigFileInfo()
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     ZConfigFileInfo* : 配置类对象
  其它说明:     无
*************************************************/
ZConfigFileInfo* ZConfigFileInfo::GetConfigFileInfoObj()
{
	static ZConfigFileInfo* spConfigFileInfoObj = NULL;

	if(spConfigFileInfoObj == NULL)
	{
		spConfigFileInfoObj = new ZConfigFileInfo;
	}
	return spConfigFileInfoObj;
}
