/*********************************************************************************************
	文件名称：	inithiseventsprocessdlg.cxx
	内    容：	初始化历史事项进程对话框类(InitHisEventsProcessDlg)实现
	功    能：	初始化历史事项，显示进程。
	主要函数：	1. init()：私有成员函数
                功能：对话框显示初始化
				2. languageChange()：私有成员函数
                功能：语言翻译函数
				3. customEvent()：保护成员函数
                功能：自定义消息响应函数

	其它说明：	无
	修改日志：	无
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
  函数名称:		init()
  功    能:     对话框显示初始化
  被调函数:     1, 构造函数 
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     void
  其它说明:     无
*************************************************/
void InitHisEventsProcessDlg::init()
{
	m_processDlg.setupUi(new InitHisEventsProcessDlg());
}

/*************************************************
  函数名称:		languageChange()
  功    能:     语言翻译函数
  被调函数:     无  
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     无
  输出参数:     无
  函数返回:     void
  其它说明:     无
*************************************************/
void InitHisEventsProcessDlg::languageChange()
{
	QString qstr;
}

/*************************************************
  函数名称:		customEvent()
  功    能:     自定义消息响应函数
  被调函数:     无  
  调用函数:     无
  访问的表:		无
  修改的表:		无
  输入参数:     1. QCustomEvent* e : 自定义消息
  输出参数:     无
  函数返回:     void
  其它说明:     无
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