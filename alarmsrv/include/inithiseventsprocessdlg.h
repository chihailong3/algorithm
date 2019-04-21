/*********************************************************************************************
	文件名称：	inithiseventsprocessdlg.h
	内    容：	初始化历史事项进程对话框类(InitHisEventsProcessDlg)声明
	功    能：	初始化历史事项，显示进程。
	主要函数：	1. init()：私有成员函数
                功能：对话框显示初始化
				2. languageChange()：私有成员函数
                功能：语言翻译函数
				3. customEvent()：保护成员函数
                功能：自定义消息响应函数

	其它说明：	包含qinithiseventsprocessdlg.h(该文件在编译过程由编译器通过qinithiseventsprocessdlg.ui文件
				自动生成)
	修改日志：	无
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