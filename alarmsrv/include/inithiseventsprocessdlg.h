/*********************************************************************************************
	�ļ����ƣ�	inithiseventsprocessdlg.h
	��    �ݣ�	��ʼ����ʷ������̶Ի�����(InitHisEventsProcessDlg)����
	��    �ܣ�	��ʼ����ʷ�����ʾ���̡�
	��Ҫ������	1. init()��˽�г�Ա����
                ���ܣ��Ի�����ʾ��ʼ��
				2. languageChange()��˽�г�Ա����
                ���ܣ����Է��뺯��
				3. customEvent()��������Ա����
                ���ܣ��Զ�����Ϣ��Ӧ����

	����˵����	����qinithiseventsprocessdlg.h(���ļ��ڱ�������ɱ�����ͨ��qinithiseventsprocessdlg.ui�ļ�
				�Զ�����)
	�޸���־��	��
*********************************************************************************************/

#ifndef _INIT_HIS_EVENTS_PROCESS_DLG_H_
#define _INIT_HIS_EVENTS_PROCESS_DLG_H_

#include "ui_qinithiseventsprocessdlg.h"

#include <qevent.h> 

#define INIT_HIS_EVENT_START	(QEvent::User+100)
#define INIT_HIS_EVENT_NUM		(QEvent::User+101)
#define INIT_HIS_EVENT_END		(QEvent::User+102)

class QEvent;
class InitHisEventsProcessDlg : public QDialog /* qInitHisEventsProcessDlg 2016/12/9 H9000V6 yujj */
{
    Q_OBJECT

	void init();
    void languageChange();

public:
    InitHisEventsProcessDlg( QWidget* parent = 0, const char* name = 0, bool modal = false/*FALSE*/, Qt::WindowFlags/*WFlags 2012/12/9 H9000V6 yujj */ fl = 0 );
    ~InitHisEventsProcessDlg();

protected:
	void customEvent ( QEvent/*QCustomEvent*/* e);

private:
	Ui::qInitHisEventsProcessDlg m_processDlg;
};


#endif//_INIT_HIS_EVENTS_PROCESS_DLG_H_