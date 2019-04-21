/*********************************************************************************************
	�ļ����ƣ�	inithiseventsprocessdlg.cxx
	��    �ݣ�	��ʼ����ʷ������̶Ի�����(InitHisEventsProcessDlg)ʵ��
	��    �ܣ�	��ʼ����ʷ�����ʾ���̡�
	��Ҫ������	1. init()��˽�г�Ա����
                ���ܣ��Ի�����ʾ��ʼ��
				2. languageChange()��˽�г�Ա����
                ���ܣ����Է��뺯��
				3. customEvent()��������Ա����
                ���ܣ��Զ�����Ϣ��Ӧ����

	����˵����	��
	�޸���־��	��
*********************************************************************************************/
#include "inithiseventsprocessdlg.h"

#include <qlineedit.h>
#include <qprogressbar.h> 
#include <QEvent>
#include <ui_qinithiseventsprocessdlg.h>

InitHisEventsProcessDlg::InitHisEventsProcessDlg( QWidget* parent, const char* name, bool modal,
	Qt::WindowFlags/* WFlags */ fl)
	/* :qInitHisEventsProcessDlg(parent, name, modal, fl) 2012/12/9 H9000V6 yujj */
	:QDialog(parent, fl)
{
	init();
}

InitHisEventsProcessDlg::~InitHisEventsProcessDlg()
{
}

/*************************************************
  ��������:		init()
  ��    ��:     �Ի�����ʾ��ʼ��
  ��������:     1, ���캯�� 
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     void
  ����˵��:     ��
*************************************************/
void InitHisEventsProcessDlg::init()
{
	m_processDlg.setupUi(new InitHisEventsProcessDlg());
}

/*************************************************
  ��������:		languageChange()
  ��    ��:     ���Է��뺯��
  ��������:     ��  
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     ��
  �������:     ��
  ��������:     void
  ����˵��:     ��
*************************************************/
void InitHisEventsProcessDlg::languageChange()
{
	QString qstr;
}

/*************************************************
  ��������:		customEvent()
  ��    ��:     �Զ�����Ϣ��Ӧ����
  ��������:     ��  
  ���ú���:     ��
  ���ʵı�:		��
  �޸ĵı�:		��
  �������:     1. QCustomEvent* e : �Զ�����Ϣ
  �������:     ��
  ��������:     void
  ����˵��:     ��
*************************************************/
void InitHisEventsProcessDlg::customEvent ( QEvent/*CustomEvent*/* e)
{
	if(e == NULL) 
		return;

	int* pData = NULL;

	switch(e->type())
	{
	case INIT_HIS_EVENT_START:
		/*
		pData = (int*)e->data();
		m_LineEditAllObject->setText(QString("%1").arg(*pData));
		m_LineEditReadObject->setText(QString("0"));
		m_ProgressBarEvents->setTotalSteps(*pData);
		*/ // 2016/12/9 H9000V6 yujj

		pData = (int*)e/*->data()*/;
		m_processDlg.m_LineEditAllObject->setText(QString("%1").arg(*pData));
		m_processDlg.m_LineEditReadObject->setText(QString("0"));
		/* m_processDlg.m_ProgressBarEvents->setTotalSteps(*pData); */
		break;
	case INIT_HIS_EVENT_NUM:
		/*
		pData = (int*)e->data();
		m_LineEditReadObject->setText(QString("%1").arg(*pData));
		m_ProgressBarEvents->setProgress(*pData);
		*/ // 2016/12/9 H9000V6 yujj

		pData = (int*)e/*->data()*/;
		m_processDlg.m_LineEditReadObject->setText(QString("%1").arg(*pData));
		/* m_processDlg.m_ProgressBarEvents->setProgress(*pData); */
		break;
	case INIT_HIS_EVENT_END:
		hide();
		break;
	default:
		break;
	}

	if(pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
}