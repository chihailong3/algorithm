#ifndef _ALARM_THREAD_H_
#define _ALARM_THREAD_H_

#include "netdef.h"

/* #include <qptrlist.h> //2016/12/9 H9000V6 yujj */
#include <QList>
#include <QSemaphore>

#if defined(_AIX) && defined(__GNUC__)
#include "dfthread.h"
#define QThread DFThread
#else
#include <qthread.h>
#endif

#ifndef NO_SELF_LANG_TR
#include "langtr.h"
#else
#define _LANGBEGIN
#define _LANGEND
#define _LANGTR(a)		a
#endif

#define TABLE_STR_REALEVENTRECORDS	_LANGBEGIN"实时事项记录表"_LANGEND

const int cLargeStackSize = 1024*2048;

class CheckDBThread : public QThread 
{
public:
	CheckDBThread() : m_bExitF(false) {}
	virtual void run();
	void stop();

private:
	bool m_bExitF;
};

/*********************************************************************************************
	类 名 称：	CheckParentThread
	内    容：	进程检测线程类(CheckParentThread)声明
	功    能：	循环检测进程
	主要函数：	1. run()：公共成员函数
                功能：启动运行线程
				2. stop()：公共成员函数
                功能：停止线程
	其它说明：	无
	修改日志：	无
*********************************************************************************************/
class CheckParentThread : public QThread 
{
public:
	CheckParentThread();

	void run();
	void stop();
};
void StartCheckParentThread();
void CloseCheckParentThread();

/*********************************************************************************************
	类 名 称：	OperateHisDBThread
	内    容：	处理历史库线程类(OperateHisDBThread)声明
	功    能：	对历史库进行增加、更新、删除操作
	主要函数：	1. run()：公共成员函数
                功能：启动运行线程
				2. stop()：公共成员函数
                功能：停止线程
				3. getHisEventsList()：公共成员函数
                功能：获得保存事项链表
				4. getSemaphore()：公共成员函数
                功能：获得类对象互斥信号
				5. appendOneHisEvent()：公共成员函数
                功能：增加一条事项到保存事项链表
				6. saveOneEventToHisDB()：私有成员函数
                功能：保存一条事项到数据库
	其它说明：	无
	修改日志：	无
*********************************************************************************************/
typedef QList/*QPtrList*/<HIS_EVENT> QHISEventList;
class OperateHisDBThread : public QThread 
{
	QHISEventList* m_HisEventsList;	//更新历史库历史事项链表
	QSemaphore* m_semaphore;

	void saveOneEventToHisDB(HIS_EVENT* pBuf);
public:
#ifdef __alpha
	OperateHisDBThread();
#else
	OperateHisDBThread(unsigned int stackSize=cLargeStackSize);
#endif

	void run();
	void stop();

	QHISEventList* getHisEventsList() {return m_HisEventsList;}
	QSemaphore* getSemaphore() {return m_semaphore;}
	void appendOneHisEvent(HIS_EVENT* pBuf, bool bInitHisEntF = false);
};
void StartOperateHisDBThread();
void CloseOperateHisDBThread();

/*********************************************************************************************
	类 名 称：	TreatEveThread
	内    容：	处理事项线程类(TreatEveThread)声明
	功    能：	处理网络接收事项
	主要函数：	1. run()：公共成员函数
                功能：启动运行线程
				2. stop()：公共成员函数
                功能：停止线程
				3. getRxList()：公共成员函数
                功能：获得接收事项链表
				4. getSemaphore()：公共成员函数
                功能：获得类对象互斥信号
				5. appendOneSysEvent()：公共成员函数
                功能：增加一条事项到接收事项链表
				6. translateSysEntToHisEnt()：私有成员函数
                功能：转换系统事项结构到历史事项保存结构
	其它说明：	无
	修改日志：	无
*********************************************************************************************/
typedef QList/*QPtrList*/<SYS_EVENT> QSYSEventList;

class TreatEveThread : public QThread 
{
	QSYSEventList* m_RxList;	//网络接收事项缓冲区
	QSemaphore* m_semaphore;

	void translateSysEntToHisEnt(SYS_EVENT* pSysEnt,HIS_EVENT* pHisEnt);

public:
#ifdef __alpha
	TreatEveThread();
#else
	TreatEveThread(unsigned int stackSize=cLargeStackSize);
#endif

	void run();
	void stop();

	QSYSEventList* getRxList() {return m_RxList;}
	QSemaphore* getSemaphore() {return m_semaphore;}

	void appendOneSysEvent(SYS_EVENT* pBuf);
	void treatOneSysEvent(SYS_EVENT* pBuf);
};
void StartTreatEventThread();
void CloseTreatEventThread();

/*********************************************************************************************
	类 名 称：	RecevieEveThread
	内    容：	接收事项线程类(RecevieEveThread)声明
	功    能：	接收事项
	主要函数：	1. run()：公共成员函数
                功能：启动运行线程
				2. stop()：公共成员函数
                功能：停止线程
				3. newDebugSysEvent()：私有成员函数
                功能：调试态生成新的系统事项
	其它说明：	无
	修改日志：	无
*********************************************************************************************/
typedef QList/*QPtrList*/<SYS_EVENT> QSYSEventList;

class RecevieEveThread : public QThread 
{
	bool newDebugSysEvent(SYS_EVENT* pBuf);
public:
#ifdef __alpha
	RecevieEveThread();
#else
	RecevieEveThread(unsigned int stackSize=cLargeStackSize);
#endif

	void run();
	void stop();
};
void StartReceiveEventThread();
void CloseReceiveEventThread();

/*********************************************************************************************
	类 名 称：	InitHisEventsThread
	内    容：	初始化历史事项线程类(InitHisEventsThread)声明
	功    能：	按配置对历史事项进行初始化
	主要函数：	1. run()：公共成员函数
                功能：启动运行线程
				2. stop()：公共成员函数
                功能：停止线程
				3. setParentWidget()：公共成员函数
                功能：设置信号发送对象(界面父窗口)
				4. InitSpecialHisEntsForBaoJi()：私有成员函数
                功能：初始化宝鸡现场特殊历史事项
				5. ReadOneHisEventByObjCode()：私有成员函数
                功能：根据条件读取一条历史事项
	其它说明：	无
	修改日志：	无
*********************************************************************************************/
class InitHisEventsThread : public QThread 
{
	QWidget* m_ParentDlg;

	void InitSpecialHisEntsForBaoJi();
	bool ReadOneHisEventByObjCode(HIS_EVENT* pBuf, char* pObjCode, char* pEntTypeStr);

public:
#ifdef __alpha
	InitHisEventsThread();
#else
	InitHisEventsThread(unsigned int stackSize=cLargeStackSize);
#endif

	void run();
	void stop();

	void setParentWidget(QWidget* pParentDlg){m_ParentDlg = pParentDlg; }
};
void StartInitHisEventsThread(QWidget* pProcessDlg);
void CloseInitHisEventsThread();



#endif//_ALARM_THREAD_H_