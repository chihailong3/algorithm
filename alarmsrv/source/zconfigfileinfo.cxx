/*********************************************************************************************
	�ļ����ƣ�	zconfigfileinfo.cxx
	��    �ݣ�	�����ļ���Ϣ��������(ZConfigFileInfo)ʵ��
	��    �ܣ�	��ȡ������Ϣ����ȡ��ʼ��������Ϣ����������������ж�
	��Ҫ������	1. GetInitObjectBuf()��������Ա����
                ���ܣ���ó�ʼ����ʷ�����Ӧ��ң�Ŷ���
				2. GetInitEntTypeStr()��������Ա����
                ���ܣ���ó�ʼ����ʷ�������������ַ���
				3. IsMeetCondition()��������Ա����
                ���ܣ��ж������Ƿ����㱣������
				4. GetConfigFileInfoObj()����̬��Ա����
                ���ܣ�������������
				5. ReadConfigFile()��˽�г�Ա����
                ���ܣ���ȡ�����ļ���Ϣ
				6. ReadInitObjectInfo()��˽�г�Ա����
                ���ܣ���ȡ��ʼ����ʷ�����Ӧ��ң�Ŷ���
				7. IsMeetEntType()��˽�г�Ա����
                ���ܣ��ж����������Ƿ����㱣������
				8. IsMeetEntObject()��˽�г�Ա����
                ���ܣ��ж���������Ƿ����㱣������
				9. GetRdbTableNameByObjectType()��˽�г�Ա����
                ���ܣ�ͨ���������ͻ��ʵʱ�����
				10. IsProcModeMeetCondition()��˽�г�Ա����
                ���ܣ��ж�ң�Ŷ��󱨾�����ʽ�Ƿ����㱣������
				11. GetEntProcModePara()��˽�г�Ա����
                ���ܣ���������ʽ����
				12. SetReloadEntProcModePara()��������Ա����
                ���ܣ��������¶�ȡ�����ʽ����

	����˵����	��
	�޸���־��	��
*********************************************************************************************/

#include "zconfigfileinfo.h"
#include "selfini.h"
#include "dorbase.h"

#define Is_YX_Scada_Event(a)	(a >= ENTTYPE_STATUS && a <= ENTTYPE_YUGAOSOE)

//�����ң�ŷ֡��ϱ�����salarm��ң�ŷֺϱ���Ӧ��һ��
const int ciYxOffAlarm  = 11;		//ң�ŷֱ���
const int ciYxOnAlarm  = 12;		//ң�źϱ���

const int ciProcModeCoef = 1000;	//ͨ��state_value2���㱨������ʽ��ϵ��

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
  ��������:		ReadConfigFile()
  ��    ��:     ��ȡ�����ļ���Ϣ
  ��������:     1, ���캯�� 
  ���ú���:     1. CSelfIni::GetAppList()
				2. CSelfIni::GetFirstByNo()
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     void
  ����˵��:     ��
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
	selfIniObj.GetAppList("[��ʼ����������]");

	pTmpStr = selfIniObj.GetFirstByNo(i);
	while(pTmpStr != NULL)
	{
		m_pObjectTypeBuf = (int*)realloc(m_pObjectTypeBuf, sizeof(int)*(m_iObjectTypeNum+1));
		*(m_pObjectTypeBuf+m_iObjectTypeNum) = atoi(pTmpStr);

		m_iObjectTypeNum++;
		i++;
		pTmpStr = selfIniObj.GetFirstByNo(i);
	}

	selfIniObj.GetAppList("[������������]");
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
			//ENTTYPE_PROTSIGSOE,	//�����ź�SOE
			//ENTTYPE_YUGAOSOE,	//Ԥ���ź�SOE
			ENTTYPE_PROTSIG,	//�����źű�λ
			ENTTYPE_YUGAO		//Ԥ���źű�λ
			);
	}
}

/*************************************************
  ��������:		ReadInitObjectInfo()
  ��    ��:     ��ȡ��ʼ����ʷ�����Ӧ��ң�Ŷ���
  ��������:     1. ���캯�� 
				2. GetInitObjectBuf()
  ���ú���:     1. Rsqlclient::selectfrom()
				2. Rsqlclient::selectattr()
				3. Rsqlclient::where()
				4. Rsqlclient::select()
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     void
  ����˵��:     ��
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
	{//��ȡ�������ʼ������
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
  ��������:		GetInitObjectBuf()
  ��    ��:     ��ó�ʼ����ʷ�����Ӧ��ң�Ŷ���
  ��������:     1. InitHisEventsThread::run()
				2. IsMeetCondition()
  ���ú���:     1. ReadInitObjectInfo()
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. bool bReloadF : �Ƿ����¶�ȡ��ʼ��ң�Ŷ���
  �������:     1. int& iInitObjNum : ��ʼ��ң�Ŷ������
  ��������:     InitObjectInfoS* : ��ʼ��ң�Ŷ��󻺳���
  ����˵��:     ��
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
  ��������:		GetInitEntTypeStr()
  ��    ��:     ��ó�ʼ����ʷ�������������ַ���
  ��������:     1. InitHisEventsThread::run()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     char* : ��ʼ����ʷ�������������ַ���
  ����˵��:     ��
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
  ��������:		GetRdbTableNameByObjectType()
  ��    ��:     ͨ���������ͻ��ʵʱ�����
  ��������:     1. ReadInitObjectInfo()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. int iObjType : ��������
  �������:     ��
  ��������:     const char* : ʵʱ�����
  ����˵��:     ��
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
  ��������:		IsMeetCondition()
  ��    ��:     �ж������Ƿ����㱣������
  ��������:     1. TreatEveThread::treatOneSysEvent()
  ���ú���:     1. GetInitObjectBuf()
				2. IsMeetEntType()
				3. IsMeetEntObject()
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. SYS_EVENT* pBuf : ϵͳ�������
  �������:     ��
  ��������:     bool : true,��������;false,����������
  ����˵��:     ��
*************************************************/
bool ZConfigFileInfo::IsMeetCondition(SYS_EVENT* pBuf)
{
	if(pBuf == NULL) return false;

	int iInitObjNum;

	//����ң�Ų������ض�����
	if(pBuf->event_sort == EVENT_OPERATION
		&& pBuf->event_type == OPTYPE_RDB_LOADTABLE)
	{//����ʵʱ��
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
  ��������:		IsMeetEntType()
  ��    ��:     �ж����������Ƿ����㱣������
  ��������:     1. IsMeetCondition()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     int iEntType : ��������
  �������:     ��
  ��������:     bool : true,��������;false,����������
  ����˵��:     ��
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
  ��������:		IsMeetEntObject()
  ��    ��:     �ж���������Ƿ����㱣������
  ��������:     1. IsMeetCondition()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. SYS_EVENT* pBuf : ϵͳ�������
  �������:     ��
  ��������:     bool : true,��������;false,����������
  ����˵��:     ��
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
  ��������:		IsProcModeMeetCondition()
  ��    ��:     �ж�ң�Ŷ��󱨾�����ʽ�Ƿ����㱣������
  ��������:     1. ReadInitObjectInfo()
				2. IsMeetEntObject()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. const int & procmode : ��������ʽ
				1. const int & state : ����״̬
  �������:     ��
  ��������:     bool : true,��������;false,����������
  ����˵��:     ��
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
			{//�ֱ�����״̬Ϊ0������true
				return true;
			}
			else if((m_pEventProcModeBuf+i)->alarm_mode == ciYxOnAlarm
				&& state == 1)
			{//�ϱ�����״̬Ϊ1������true
				return true;
			}
			break;
		}
	}
	return false;
}

/*************************************************
  ��������:		GetEntProcModePara()
  ��    ��:     ��������ʽ����
  ��������:     1. IsProcModeMeetCondition()
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     ��
  ����˵��:     ��
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
  ��������:		GetConfigFileInfoObj()
  ��    ��:     ������������
  ��������:     1. TreatEveThread::treatOneSysEvent()
				2. InitHisEventsThread::run()
  ���ú���:     1. ZConfigFileInfo()
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     ZConfigFileInfo* : ���������
  ����˵��:     ��
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
