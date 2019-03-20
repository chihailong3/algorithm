#include "alarmthread.h"
#include "inithiseventsprocessdlg.h"
#include "zconfigfileinfo.h"
#include "sybase.h"

#include <qapplication.h>
#include <qdatetime.h>
#include <QEvent>

extern bool g_bInitHisEnts;
extern bool g_bDebugState;
static bool sExitFlag = true;
static bool sbMainServer = false;

/************数据库长连接检测线程************/
void CheckDBThread::run()
{
	while( !m_bExitF )
	{
		dbfchecklink();
		msleep(1000);
	}
	return;
}
void CheckDBThread::stop()
{
	m_bExitF = true;
}

/******************************进程检测线程类******************************/
CheckParentThread	*g_pCheckParentThread=NULL;
CheckParentThread::CheckParentThread()
	:QThread()
{
}

void CheckParentThread::run()
{
	proc_man  procman;
	net_cfg NetCfg;
	while(sExitFlag)
	{
		dbfchecklink();	//数据库连接检查
		if(NetCfg.am_I_main_server("SCADA")) 
		{
			sbMainServer = TRUE;
		}
		else
		{
			sbMainServer = FALSE;
		}
		if(!procman.CheckParentProc())
		{
			exit();
		}
		msleep(1000); 
	}
	return;
}

void CheckParentThread::stop()
{
	sExitFlag = false;
	return;
}

void StartCheckParentThread()
{
	if(g_pCheckParentThread == NULL) g_pCheckParentThread = new CheckParentThread();
	g_pCheckParentThread->start();
}

void CloseCheckParentThread()
{
	if(g_pCheckParentThread && g_pCheckParentThread->isRunning()/* running() 2012/12/9 H9000V6 yujj */) 
	{
		g_pCheckParentThread->stop();        
		g_pCheckParentThread->wait();
	}
	if(g_pCheckParentThread != NULL)
	{
		delete g_pCheckParentThread;
		g_pCheckParentThread=NULL;
	}
}
/******************************进程检测线程类******************************/

/******************************处理历史库线程类******************************/
OperateHisDBThread	*g_pOperateHisDBThread=NULL;
#ifdef __alpha
OperateHisDBThread::OperateHisDBThread()
	:QThread()
#else
OperateHisDBThread::OperateHisDBThread(unsigned int stackSize)
	:QThread(/* stackSize 2012/12/9 H9000V6 yujj */)
#endif
{
	m_HisEventsList = new QHISEventList;
	/* m_HisEventsList->setAutoDelete( true ); 2012/12/9 H9000V6 yujj */
	m_HisEventsList->clear();
    m_semaphore = new QSemaphore(1);
    while (m_semaphore->available() < m_semaphore->available()/*total() 2012/12/9 H9000V6 yujj */)
	{
		/* (*m_semaphore)--; 2016/12/9 H9000V6 yujj */
	}
}

void OperateHisDBThread::run()
{
	while(sExitFlag)
	{
		while(sExitFlag && m_HisEventsList != NULL && !m_HisEventsList->isEmpty())
		{
			/* HIS_EVENT* pHisEvent = (HIS_EVENT*)m_HisEventsList->first(); 2016/12/9 H9000V6 yujj */
			HIS_EVENT* pHisEvent = &(HIS_EVENT)m_HisEventsList->first();
			saveOneEventToHisDB(pHisEvent);
			/* (*m_semaphore)++; 2016/12/9 H9000V6 yujj */
			m_HisEventsList->removeFirst();
			/* (*m_semaphore)--; 2016/12/9 H9000V6 yujj */
		}
			
		msleep(50);
	}
	return;
}

void OperateHisDBThread::appendOneHisEvent(HIS_EVENT* pBuf, bool bInitHisEntF)
{
	if(pBuf == NULL) return;

	if(!sbMainServer && !bInitHisEntF) return;	//非主服务器且不是初始化历史事项，不保存

	HIS_EVENT* pNewEnt = new HIS_EVENT;
	memcpy(pNewEnt, pBuf, sizeof(HIS_EVENT));

	/* (*m_semaphore)++; 2016/12/9 H9000V6 yujj */
	/* m_HisEventsList->append(pNewEnt); 2016/12/9 H9000V6 yujj */
	m_HisEventsList->append(*pNewEnt);
	/* (*m_semaphore)--; 2016/12/9 H9000V6 yujj */
}

void OperateHisDBThread::saveOneEventToHisDB(HIS_EVENT* pBuf)
{

	if(pBuf == NULL) return;

    FUNC_STR        func;
    CS_DATAFMT      *datafmt=NULL;
	CS_RETCODE		retcode;

    func.serverno = SERVER_DEFAULT;	
	sprintf(func.table, "xopenshdb.dbo.%s", _LANGTR(TABLE_STR_REALEVENTRECORDS));

	//删除原有事项
	func.func = ISQL_COMMAND;
	sprintf(func.isql, "delete %s where 事件类型=%d and 事件对象名0='%s'",
		_LANGTR(TABLE_STR_REALEVENTRECORDS), pBuf->event_type, pBuf->member_name0);
	retcode = dbfisqlcommand( &func);

	//增加新事项
	func.func = GET_TABLE_INFO;
	retcode = dbfgettableinfo(&func,&datafmt);
    if (retcode != CS_SUCCEED || func.colnum<1)
	{
		if(datafmt) free(datafmt); datafmt = NULL;
		printf("dbfgettableinfo(%s) fail\n", func.table);
        return ;
	}

	strcpy(func.isql, "");
	func.roxnum = 1;
	func.func = INSERT_RECORDERS;
	retcode = dbfinsertrecorders(&func,datafmt,(HIS_EVENT*)pBuf);

	if(datafmt)
	{
		free(datafmt);
		datafmt = NULL;
	}
	return;
}

void OperateHisDBThread::stop()
{
	sExitFlag = false;
	return;
}

void StartOperateHisDBThread()
{
	if(g_pOperateHisDBThread == NULL) g_pOperateHisDBThread = new OperateHisDBThread();
	g_pOperateHisDBThread->start();
}

void CloseOperateHisDBThread()
{
	if(g_pOperateHisDBThread && g_pOperateHisDBThread->isRunning()/* running() 2016/12/9 H9000V6 yujj */) 
	{
		g_pOperateHisDBThread->stop();        
		g_pOperateHisDBThread->wait();
	}
	if(g_pOperateHisDBThread != NULL)
	{
		QHISEventList* pHisEntsList = g_pOperateHisDBThread->getHisEventsList();
		if(pHisEntsList != NULL)
		{
			pHisEntsList->clear();
			delete pHisEntsList;
			pHisEntsList = NULL;
		}
		QSemaphore* pSemaphore = g_pOperateHisDBThread->getSemaphore();
		if(pSemaphore != NULL)
		{
			delete pSemaphore;
			pSemaphore = NULL;
		}
		delete g_pOperateHisDBThread;
		g_pOperateHisDBThread=NULL;
	}
}
/******************************处理历史库线程类******************************/

/******************************处理事项线程类******************************/
TreatEveThread	*g_pTreatEveThread=NULL;
#ifdef __alpha
TreatEveThread::TreatEveThread()
	:QThread()
#else
TreatEveThread::TreatEveThread(unsigned int stackSize)
	: QThread(/*stackSize 2016 / 12 / 9 H9000V6 yujj*/)
#endif
{
	m_RxList = new QSYSEventList;
	/* m_RxList->setAutoDelete( true );2016/12/9 H9000V6 yujj */
	m_RxList->clear();
    m_semaphore = new QSemaphore(1);
    while (m_semaphore->available() < m_semaphore->available()/*total()2016/12/9 H9000V6 yujj */)
	{
		/* (*m_semaphore)--; 2016/12/9 H9000V6 yujj */
	}
}

void TreatEveThread::run()
{
	while(sExitFlag)
	{
		while(sExitFlag && !g_bInitHisEnts && m_RxList != NULL && !m_RxList->isEmpty())
		{
			/* SYS_EVENT* pSysEvent = (SYS_EVENT*)m_RxList->first(); 2016/12/9 H9000V6 yujj */
			SYS_EVENT* pSysEvent = &(SYS_EVENT)m_RxList->first();
			treatOneSysEvent(pSysEvent);
			/* (*m_semaphore)++;2016/12/9 H9000V6 yujj */
			m_RxList->removeFirst();
			/* (*m_semaphore)--;2016/12/9 H9000V6 yujj */
		}
			
		msleep(50);
	}
	return;
}

void TreatEveThread::appendOneSysEvent(SYS_EVENT* pBuf)
{
	if(pBuf == NULL) return;

	SYS_EVENT* pNewEnt = new SYS_EVENT;
	memcpy(pNewEnt, pBuf, sizeof(SYS_EVENT));
	/*
	(*m_semaphore)++;
	m_RxList->append(pNewEnt);
	(*m_semaphore)--;
	*/ //2016/12/9 H9000V6 yujj

	*m_semaphore++;
	m_RxList->append(*pNewEnt);
	*m_semaphore--;
}

void TreatEveThread::treatOneSysEvent(SYS_EVENT* pBuf)
{
	if(pBuf == NULL) return;

	ZConfigFileInfo* pConfigFileInfoObj = ZConfigFileInfo::GetConfigFileInfoObj();
	if(pConfigFileInfoObj == NULL) return;

	if(pBuf->event_sort == EVENT_OPERATION
		&& pBuf->event_type == OPTYPE_RDB_LOADTABLE)
	{//加载实时库‘事项处理方式表I’事项
		if(strcmp(pBuf->member_name1, "eprocmode") == 0)
		{
			pConfigFileInfoObj->SetReloadEntProcModePara();
		}
	}

	if(g_bDebugState || pConfigFileInfoObj->IsMeetCondition(pBuf))
	{
		HIS_EVENT his_event;
		memset(&his_event, 0, sizeof(HIS_EVENT));
		translateSysEntToHisEnt(pBuf, &his_event);
		if(g_pOperateHisDBThread != NULL)
		{
			g_pOperateHisDBThread->appendOneHisEvent(&his_event, g_bDebugState);
		}
	}
}

void TreatEveThread::translateSysEntToHisEnt(SYS_EVENT* pSysEnt,HIS_EVENT* pHisEnt)
{
	if(pSysEnt == NULL || pHisEnt == NULL)
	{
		return;
	}

	pHisEnt->event_sort	= (unsigned short)pSysEnt->event_sort;
	pHisEnt->event_type	= (unsigned short)pSysEnt->event_type;
	pHisEnt->ymd			= (unsigned int)pSysEnt->ymd;
	pHisEnt->hmsms		= (unsigned int)pSysEnt->hmsms;
	pHisEnt->security		= (unsigned char)pSysEnt->security;
	pHisEnt->voltgrade	= (unsigned char)pSysEnt->voltgrade;
	pHisEnt->testflag		= (unsigned char)pSysEnt->testflag;
	memcpy(&pHisEnt->output, &pSysEnt->output, 1);
	pHisEnt->state_value	= (unsigned short)pSysEnt->state_value; 
	pHisEnt->state_value1	= (unsigned short)pSysEnt->state_value1; 
	pHisEnt->state_value2	= (unsigned short)pSysEnt->state_value2; 
	pHisEnt->float_value	= (float)pSysEnt->float_value; 
	pHisEnt->float_value1	= (float)pSysEnt->float_value1; 
	pHisEnt->float_value2	= (float)pSysEnt->float_value2; 
	pHisEnt->float_value3	= (float)pSysEnt->float_value3; 
	strcpy(pHisEnt->group_name, pSysEnt->group_name);
	strcpy(pHisEnt->member_name0, pSysEnt->member_name0);
	strcpy(pHisEnt->member_name1, pSysEnt->member_name1);
	strcpy(pHisEnt->member_name2, pSysEnt->member_name2);
	strcpy(pHisEnt->member_name3, pSysEnt->member_name3);
	strcpy(pHisEnt->char_info, pSysEnt->char_info);
	strcpy(pHisEnt->confirmer, "");
	pHisEnt->confirmdate = 0;
	pHisEnt->confirmtime = 0;
	strcpy(pHisEnt->jiange_name, pSysEnt->jiange_name);
}

void TreatEveThread::stop()
{
	sExitFlag = false;
	return;
}

void StartTreatEventThread()
{
	if(g_pTreatEveThread == NULL) g_pTreatEveThread = new TreatEveThread();
	g_pTreatEveThread->start();
}

void CloseTreatEventThread()
{
	if(g_pTreatEveThread && g_pTreatEveThread->isRunning() /*running() 2016/12/9 H9000V6 yujj*/) 
	{
		g_pTreatEveThread->stop();        
		g_pTreatEveThread->wait();
	}
	if(g_pTreatEveThread != NULL)
	{
		QSYSEventList* pRxList = g_pTreatEveThread->getRxList();
		if(pRxList != NULL)
		{
			pRxList->clear();
			delete pRxList;
			pRxList = NULL;
		}
		QSemaphore* pSemaphore = g_pTreatEveThread->getSemaphore();
		if(pSemaphore != NULL)
		{
			delete pSemaphore;
			pSemaphore = NULL;
		}
		delete g_pTreatEveThread;
		g_pTreatEveThread=NULL;
	}
}
/******************************处理事项线程类******************************/

/******************************接收事项线程类******************************/
RecevieEveThread	*g_pRecieveEveThread=NULL;
#ifdef __alpha
RecevieEveThread::RecevieEveThread()
	:QThread()
#else
RecevieEveThread::RecevieEveThread(unsigned int stackSize)
	:QThread(/* stackSize 2016/12/9 H9000V6 yujj */)
#endif
{
}

void RecevieEveThread::run()
{
	SYS_EVENT sys_event;
	static event_proc EventProc;

	while(sExitFlag)
	{
		if(g_bDebugState)
		{//调试态
			if(newDebugSysEvent(&sys_event))
			{
				if(g_pTreatEveThread != NULL)
				{
					g_pTreatEveThread->appendOneSysEvent(&sys_event);
				}
			}
			msleep(1000);
		}
		else
		{
			if(EventProc.read_event(&sys_event)==1)
			{
				if(g_pTreatEveThread != NULL)
				{
					g_pTreatEveThread->appendOneSysEvent(&sys_event);
				}
			}
			else
			{
				msleep(50);
			}
		}
	}
	return;
}

void RecevieEveThread::stop()
{
	sExitFlag = false;
	return;
}

bool RecevieEveThread::newDebugSysEvent(SYS_EVENT* pBuf)
{
	static int siCount = 0;

	if(pBuf == NULL) return false;
	//if(siCount == 10) return false;

	QDateTime qdt = QDateTime::currentDateTime();
	int iYMD = qdt.date().year()*10000 + qdt.date().month()*100 + qdt.date().day();
	int iHMSMS = qdt.time().hour()*10000*1000 + qdt.time().minute()*1000*100 + qdt.time().second()*1000 + qdt.time().msec();
	
	memset(pBuf, 0, sizeof(SYS_EVENT));

	pBuf->event_sort	= EVENT_SCADA;
	pBuf->event_type	= ENTTYPE_BREAK;
	pBuf->ymd			= iYMD;
	pBuf->hmsms			= iHMSMS;
	pBuf->state_value	= siCount/2; 
	strcpy(pBuf->group_name, "dg");
	strcpy(pBuf->member_name0, "debug_cb");
	if(siCount/2 == 0)
		strcpy(pBuf->char_info, "debug_cb 开关分");
	else
		strcpy(pBuf->char_info, "debug_cb 开关合");

	siCount++;
	return true;
}

void StartReceiveEventThread()
{
	if(g_pRecieveEveThread == NULL) g_pRecieveEveThread = new RecevieEveThread();
	g_pRecieveEveThread->start();
}

void CloseReceiveEventThread()
{
	if(g_pRecieveEveThread && g_pRecieveEveThread->isRunning()/*running() 2016/12/9 H9000V6 yujj*/) 
	{
		g_pRecieveEveThread->stop();        
		g_pRecieveEveThread->wait();
	}
	if(g_pRecieveEveThread != NULL)
	{
		delete g_pRecieveEveThread;
		g_pRecieveEveThread=NULL;
	}
}
/******************************接收事项线程类******************************/


/******************************初始化历史事项线程类******************************/
InitHisEventsThread	*g_pInitHisEventsThread=NULL;
#ifdef __alpha
InitHisEventsThread::InitHisEventsThread()
	:QThread()
#else
InitHisEventsThread::InitHisEventsThread(unsigned int stackSize)
	:QThread(/* stackSize 2016/12/9 H9000V6 yujj */)
#endif
{
}

void InitHisEventsThread::run()
{
#ifdef BAOJI_VERSION	//宝鸡特殊初始化事项
	InitSpecialHisEntsForBaoJi();
#endif
	ZConfigFileInfo* pConfigFileInfoObj = ZConfigFileInfo::GetConfigFileInfoObj();
	if(pConfigFileInfoObj == NULL) 
	{
		g_bInitHisEnts = false;
		if(m_ParentDlg != NULL)
		{
			QEvent/*QCustomEvent*/* pEvent = new QEvent/*QCustomEvent*/((QEvent::Type)INIT_HIS_EVENT_END);
			QApplication::postEvent(m_ParentDlg, pEvent);
		}
		printf("########Initializing history events end! \n");
		return;
	}

	int iInitObjNum = 0;
	InitObjectInfoS* pInitObjInfoBuf = pConfigFileInfoObj->GetInitObjectBuf(iInitObjNum);
	if(iInitObjNum == 0 || pInitObjInfoBuf == NULL) 
	{
		g_bInitHisEnts = false;
		if(m_ParentDlg != NULL)
		{
			QEvent/*QCustomEvent*/* pEvent = new QEvent/*QCustomEvent*/((QEvent::Type)INIT_HIS_EVENT_END);
			QApplication::postEvent(m_ParentDlg, pEvent);
		}
		printf("########Initializing history events end! \n");
		return;
	}

	char* pEntTypeStr = pConfigFileInfoObj->GetInitEntTypeStr();

	if(m_ParentDlg != NULL)
	{
		int* pTmpValue = new int;
		*pTmpValue = iInitObjNum;
		QEvent/*QCustomEvent*/* pEvent = new QEvent/*QCustomEvent*/((QEvent::Type)INIT_HIS_EVENT_START/*, pTmpValue */);
		QApplication::postEvent(m_ParentDlg, pEvent);
	}
	HIS_EVENT his_event;
	int i = 0;
	while(iInitObjNum > 0)
	{
		if(ReadOneHisEventByObjCode(&his_event, (pInitObjInfoBuf+i)->cObjCode, pEntTypeStr))
		{
			if(g_pOperateHisDBThread != NULL)
			{
				g_pOperateHisDBThread->appendOneHisEvent(&his_event, true);
			}
		}
		i++;
		iInitObjNum--;
		if(m_ParentDlg != NULL)
		{
			int* pTmpValue = new int;
			*pTmpValue = i;
			/* QCustomEvent* pEvent = new QCustomEvent((QEvent::Type)INIT_HIS_EVENT_NUM, pTmpValue); 2012/12/9 H9000V6 yujj */
			QEvent* pEvent = new QEvent((QEvent::Type)INIT_HIS_EVENT_NUM/*, pTmpValue*/);
			QApplication::postEvent(m_ParentDlg, pEvent);
		}
	}

	if(m_ParentDlg != NULL)
	{
		/* QCustomEvent* pEvent = new QCustomEvent((QEvent::Type)INIT_HIS_EVENT_END); 2012/12/9 H9000V6 yujj */
		QEvent* pEvent = new QEvent((QEvent::Type)INIT_HIS_EVENT_END);
		QApplication::postEvent(m_ParentDlg, pEvent);
	}
	g_bInitHisEnts = false;
	printf("########Initializing history events end! \n");
	return;
}

bool InitHisEventsThread::ReadOneHisEventByObjCode(HIS_EVENT* pBuf, char* pObjCode, char* pEntTypeStr)
{
	if(pBuf == NULL || pObjCode == NULL || pEntTypeStr == NULL) return false;
	int i,index=0;
	FUNC_STR        func;	
	CS_DATAFMT      *datafmt=NULL;
	func.func=GET_TABLE_INFO;
	func.serverno=SERVER_DEFAULT;
	strcpy(func.table,"xopenshdb.dbo.历史事项表");
	if(dbfgettableinfo(&func,&datafmt) != CS_SUCCEED) 
	{
		if(datafmt) free(datafmt); 
		printf("dbfgettableinfo(xopenshdb.dbo.历史事项表) fail\n");
		return false;
	}
	if(datafmt==NULL) return false;
	printf("dbfgettableinfo(xopenshdb.dbo.历史事项表) ok, colnum=%d\n",func.colnum);
	if (func.colnum<1) 
	{		
		free(datafmt);		
		return false;	
	}
	func.recorderlen=0;
	for (i=0;i<func.colnum;i++)
	{
		func.col[i].select=YES;
		func.col[i].order=NO;
		func.col[i].condition=NO;
	}

	HIS_EVENT *re=NULL;
	sprintf(
		func.isql,
		"事件类型 in %s and 事件对象名0='%s' and 年月日=(select max(年月日) from 历史事项表 where 事件类型 in %s and 事件对象名0='%s')  order by 年月日,时分秒毫秒 ", 
		pEntTypeStr, pObjCode, pEntTypeStr, pObjCode
		);
	func.func=SELECT_ISQL_RECORDERS;
	if(dbfselectrecorders(&func,datafmt,(void **)&re)!=CS_SUCCEED) 
	{
		if(re) free(re); re=NULL;
		if(datafmt) free(datafmt); datafmt=NULL;
		return false;
	}
	if(func.ret_roxnum >= 1) 
	{
		index = func.ret_roxnum-1;
	}
	else
	{
		if(re) free(re); re=NULL;
		if(datafmt) free(datafmt); datafmt=NULL;
		return false;
	}
	memcpy((HIS_EVENT*)pBuf, (HIS_EVENT*)re+index, sizeof(HIS_EVENT));

	if(re) free(re); re=NULL;
	if(datafmt) free(datafmt); datafmt=NULL;
	return true;
}

void InitHisEventsThread::InitSpecialHisEntsForBaoJi()
{
	int i;
	CS_RETCODE		retcode;
	FUNC_STR        func;	
	CS_DATAFMT      *datafmt=NULL;
	func.func=GET_TABLE_INFO;
	func.serverno=SERVER_DEFAULT;
	strcpy(func.table,"xopenshdb.dbo.历史事项表");
	if(dbfgettableinfo(&func,&datafmt) != CS_SUCCEED) 
	{
		if(datafmt) free(datafmt); 
		printf("dbfgettableinfo(xopenshdb.dbo.历史事项表) fail\n");
		return;
	}
	if(datafmt==NULL) return;
	printf("dbfgettableinfo(xopenshdb.dbo.历史事项表) ok, colnum=%d\n",func.colnum);
	if (func.colnum<1) 
	{		
		free(datafmt);		
		return;	
	}
	func.recorderlen=0;
	for (i=0;i<func.colnum;i++)
	{
			func.col[i].select=YES;
			func.col[i].order=NO;
			func.col[i].condition=NO;
	}

	sprintf(
		func.isql,
		"事件类型 = %d and 确认日期 = 0 order by 年月日,时分秒毫秒 ", 
		3005
		);
	func.func=SELECT_ISQL_RECORDERS;

	HIS_EVENT *re=NULL;
	if(dbfselectrecorders(&func,datafmt,(void **)&re)!=CS_SUCCEED) 
	{
		if(re) free(re); re=NULL;
		if(datafmt) free(datafmt); datafmt=NULL;
		return;
	}

	int iEntNum = func.ret_roxnum;
	if(iEntNum > 0)
	{
		strcpy(func.table,"xopenshdb.dbo.实时事项记录表");
		func.func = GET_TABLE_INFO;
		retcode = dbfgettableinfo(&func,&datafmt);
		if (retcode != CS_SUCCEED || func.colnum<1)
		{
			if(datafmt) free(datafmt); datafmt = NULL;
			printf("dbfgettableinfo(%s) fail\n", func.table);
			return ;
		}

		strcpy(func.isql, "");
		func.roxnum = iEntNum;
		func.func = INSERT_RECORDERS;
		dbfinsertrecorders(&func,datafmt,(HIS_EVENT*)re);
	}
	if(re) free(re); re=NULL;
	if(datafmt) free(datafmt); datafmt=NULL;
	return;
}

void InitHisEventsThread::stop()
{
	return;
}

void StartInitHisEventsThread(QWidget* pProcessDlg)
{
	if(g_pInitHisEventsThread == NULL) g_pInitHisEventsThread = new InitHisEventsThread();
	g_pInitHisEventsThread->setParentWidget(pProcessDlg);
	g_pInitHisEventsThread->start();
}

void CloseInitHisEventsThread()
{
	if(g_pInitHisEventsThread && g_pInitHisEventsThread->isRunning()/*running() 2016/12/9 H9000V6 yujj */) 
	{
		g_pInitHisEventsThread->stop();        
		g_pInitHisEventsThread->wait();
	}
	if(g_pInitHisEventsThread != NULL)
	{
		delete g_pInitHisEventsThread;
		g_pInitHisEventsThread=NULL;
	}
}
/******************************初始化历史事项线程类******************************/


