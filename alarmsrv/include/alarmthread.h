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

#define TABLE_STR_REALEVENTRECORDS	_LANGBEGIN"ʵʱ�����¼��"_LANGEND

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
	�� �� �ƣ�	CheckParentThread
	��    �ݣ�	���̼���߳���(CheckParentThread)����
	��    �ܣ�	ѭ��������
	��Ҫ������	1. run()��������Ա����
                ���ܣ����������߳�
				2. stop()��������Ա����
                ���ܣ�ֹͣ�߳�
	����˵����	��
	�޸���־��	��
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
	�� �� �ƣ�	OperateHisDBThread
	��    �ݣ�	������ʷ���߳���(OperateHisDBThread)����
	��    �ܣ�	����ʷ��������ӡ����¡�ɾ������
	��Ҫ������	1. run()��������Ա����
                ���ܣ����������߳�
				2. stop()��������Ա����
                ���ܣ�ֹͣ�߳�
				3. getHisEventsList()��������Ա����
                ���ܣ���ñ�����������
				4. getSemaphore()��������Ա����
                ���ܣ��������󻥳��ź�
				5. appendOneHisEvent()��������Ա����
                ���ܣ�����һ�����������������
				6. saveOneEventToHisDB()��˽�г�Ա����
                ���ܣ�����һ��������ݿ�
	����˵����	��
	�޸���־��	��
*********************************************************************************************/
typedef QList/*QPtrList*/<HIS_EVENT> QHISEventList;
class OperateHisDBThread : public QThread 
{
	QHISEventList* m_HisEventsList;	//������ʷ����ʷ��������
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
	�� �� �ƣ�	TreatEveThread
	��    �ݣ�	���������߳���(TreatEveThread)����
	��    �ܣ�	���������������
	��Ҫ������	1. run()��������Ա����
                ���ܣ����������߳�
				2. stop()��������Ա����
                ���ܣ�ֹͣ�߳�
				3. getRxList()��������Ա����
                ���ܣ���ý�����������
				4. getSemaphore()��������Ա����
                ���ܣ��������󻥳��ź�
				5. appendOneSysEvent()��������Ա����
                ���ܣ�����һ�����������������
				6. translateSysEntToHisEnt()��˽�г�Ա����
                ���ܣ�ת��ϵͳ����ṹ����ʷ�����ṹ
	����˵����	��
	�޸���־��	��
*********************************************************************************************/
typedef QList/*QPtrList*/<SYS_EVENT> QSYSEventList;

class TreatEveThread : public QThread 
{
	QSYSEventList* m_RxList;	//��������������
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
	�� �� �ƣ�	RecevieEveThread
	��    �ݣ�	���������߳���(RecevieEveThread)����
	��    �ܣ�	��������
	��Ҫ������	1. run()��������Ա����
                ���ܣ����������߳�
				2. stop()��������Ա����
                ���ܣ�ֹͣ�߳�
				3. newDebugSysEvent()��˽�г�Ա����
                ���ܣ�����̬�����µ�ϵͳ����
	����˵����	��
	�޸���־��	��
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
	�� �� �ƣ�	InitHisEventsThread
	��    �ݣ�	��ʼ����ʷ�����߳���(InitHisEventsThread)����
	��    �ܣ�	�����ö���ʷ������г�ʼ��
	��Ҫ������	1. run()��������Ա����
                ���ܣ����������߳�
				2. stop()��������Ա����
                ���ܣ�ֹͣ�߳�
				3. setParentWidget()��������Ա����
                ���ܣ������źŷ��Ͷ���(���游����)
				4. InitSpecialHisEntsForBaoJi()��˽�г�Ա����
                ���ܣ���ʼ�������ֳ�������ʷ����
				5. ReadOneHisEventByObjCode()��˽�г�Ա����
                ���ܣ�����������ȡһ����ʷ����
	����˵����	��
	�޸���־��	��
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