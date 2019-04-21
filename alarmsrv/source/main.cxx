#include "netdef.h"
#include "sybase.h"
#include "inithiseventsprocessdlg.h"
#include "alarmthread.h"

#ifdef	WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

/* #include <qapplication.h> //2016/12/9 H9000V6 yujj */
#include <qtextcodec.h> 
#include <qfile.h> 
#include <QList>
#include <QTranslator>
#include <QTextStream>
#include <QApplication>

bool g_bInitHisEnts = false;	//
bool g_bDebugState = false;		//

QTextCodec *g_pCodec = NULL; //编码解码器

QList/*QPtrList*/<QTranslator*> global_app_translators;
void qtpainter_InstallTranslator( QApplication & app )
{
	QString path_name;
	QString file_name;

	char* tmp_ptr = NULL;
	tmp_ptr = getenv("H9HOME");
	if( !tmp_ptr )
		file_name = "";
	else
		file_name = tmp_ptr;

	QFile file(file_name + "/uif/resource/alarmsrv.ini" );
	QTextStream text(&file);
	QString line ;
	if (!file.open( QIODevice::ReadOnly/* IO_ReadOnly //2016/12/9 H9000V6 yujj */)) {
		qDebug("\nno alarmsrv.ini file.\n");
		return;
	}

	tmp_ptr = getenv("H9HOME");
	if( !tmp_ptr )
		path_name = "";
	else
		path_name = tmp_ptr;

	global_app_translators.clear();
	/* global_app_translators.setAutoDelete( TRUE ); //2016/12/9 H9000V6 yujj */

	QTranslator *translator = NULL;
	while( !file.atEnd() ) {
		line = text.readLine();
		line.simplified();/* simplifyWhiteSpace(); //2016/12/9 H9000V6 yujj */
		if( line.length() == 0 ) continue;
		if( line.at(0) == '#' ) continue;

		translator = new QTranslator( 0 );
		translator->load( path_name + "/uif/i18n/" + line );
		app.installTranslator( translator );

		global_app_translators.append( translator );
	}
}

/**
 * @fn		QString GetResourceString( char* key )
 * @brief	根据配置项获取配置信息
 *			
 * @param	char* key : 配置项
 * @return	QString : 配置信息
 * @author	 
 * @date	2011-01-20
 * @see		读取Gui.rf文件的配置信息	     
 */
QString GetResourceString( char* key )
{
	static int nInit = 0;
    static QStringList list;
	QString qstr = QString::null;
	QString line, left, right;

	if( key == NULL ) 
	{
		return qstr;
	}

	if( nInit == 0 )
	{
		nInit = 1;

		char acFilePath[200];
		char* tmp_ptr = getenv("H9HOME");
		if( tmp_ptr != NULL ) 
		{
			sprintf(acFilePath,"%s/uif/resource/Gui.rf",tmp_ptr);
		}
		else
		{
			sprintf(acFilePath,"Gui.rf");
		}
		
		QFile file( acFilePath );

		if( file.exists() )
		{
			QTextStream text(&file);
			if( !file.open( QIODevice::ReadOnly/* IO_ReadOnly 2016/12/9 H9000V6 yujj */ ) ) 
			{
				printf("Read file [%s] failed!\n", acFilePath);
				return qstr;
			}

			while( !file.atEnd() ) 
			{
				line = text.readLine();
				line.simplified();/* simplifyWhiteSpace(); 2016/12/9 H9000V6 yujj */
				if( line.length() == 0 ) continue;
				if( line.at(0) == '#' ) continue;

				list << line;
			}
			file.close();
		}
	}

	for( int i=0; i<(int)list.count(); i++ )
	{
		line = (QString)list[i];
		left = line.section( ':', 0, 0 );
		right = line.section( ':', 1, 1 );
		if( !left.isNull() && qstricmp((const char*)left.toLatin1().data()/* left 2016/12/9 H9000V6 yujj */,
			(const char*)QString("gui.%1").arg(key).toLatin1().data()) == 0 )
		{
			qstr = right.trimmed(); /* stripWhiteSpace(); 2016/12/9 H9000V6 yujj */
			break;
		}
	}

	return qstr;
}

int main( int argc, char** argv )
{
	int res=0;
    if ( argc >= 2 ) 
	{
	    for ( int i=1; i < argc; i++ ) 
		{
#ifdef WIN32
			if (stricmp(argv[i], "-init") == 0) 
#else
			if (strcasecmp(argv[i], "-init") == 0) 
#endif
			{
				g_bInitHisEnts = true;
			}
#ifdef WIN32
			if (stricmp(argv[i], "-debug") == 0) 
#else
			if (strcasecmp(argv[i], "-debug") == 0) 
#endif
			{
				g_bDebugState = true;
			}
		}
	}

	QApplication app(argc, argv );


// #if defined(__linux) && defined(__x86_64__)
// 
// 	g_pCodec = QTextCodec::codecForName("UTF8"); 
// 	
// 	QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF8") );//设置缺省解码字体类型
// 	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF8") );//设置缺省解码字体类型
// 	QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF8") );//设置缺省解码字体类型
// 
// #else
	g_pCodec = QTextCodec::codecForName("GBK"); 
	/*
	QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );//设置缺省解码字体类型
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("GBK") );//设置缺省解码字体类型
	*/ //2016/12/9 H9000V6 yujj
	QTextCodec::setCodecForLocale( QTextCodec::codecForName("GBK") );//设置缺省解码字体类型
// #endif



	qtpainter_InstallTranslator(app);

	InitHisEventsProcessDlg *pDlg = NULL;
	if(g_bInitHisEnts)
	{
		pDlg = new InitHisEventsProcessDlg;
		/* pDlg->show(); 2016/12/9 H9000V6 yujj */
		/* app.setMainWidget(pDlg); 2016/12/9 H900V6 yujj */
	}

	int pid = getpid();
#if defined(WIN32)
	parent.set_main_wnd((HWND)pid);
#else
	parent.set_main_wnd(pid);
#endif  	
	
	proc_man procman;
	if(procman.is_proc_exist("alarmsrv")) 	
	{
		printf("Alarmsrv is running now!");
		global_app_translators.clear();
		return -1;
	}
	procman.start_proc("alarmsrv","realalarm service process","rsroot");	//实时报警服务进程

	printf("startdb(1,1) begin \n");
	startdb(1,1);

	CheckDBThread* pCheckDBThread = NULL;
	QString qstrLinkMode = GetResourceString("dbLinkMode");
	if( !qstrLinkMode.isNull() && !qstrLinkMode.isEmpty() && qstrLinkMode.toInt() == 1 )
	{
		dbfsetlinkmode(1);	//  20100912
		pCheckDBThread = new CheckDBThread();
		pCheckDBThread->start();
	}

	printf("start checkparent thread begin \n"); 
	StartCheckParentThread();	//启动CheckParentProc处理线程

	printf("start operate history DB thread begin \n"); 
	StartOperateHisDBThread();	//启动处理历史库线程

	printf("start treat event thread begin \n"); 
	StartTreatEventThread();	//启动处理事项线程

	printf("start receive event thread begin \n"); 
	StartReceiveEventThread();	//启动接收事项线程

	if(g_bInitHisEnts)
	{
		printf("start init history events thread begin \n"); 
		StartInitHisEventsThread(pDlg);	//启动出始化历史事项线程
	}

	res = app.exec();

	if( pCheckDBThread != NULL )
	{
		if (pCheckDBThread->isRunning()/* running() 2012/12/9 H9000V6 yujj */) 
		{
			pCheckDBThread->stop();        
			pCheckDBThread->wait();
		}
		delete pCheckDBThread;
		pCheckDBThread=NULL;
	}

	CloseInitHisEventsThread();
	CloseReceiveEventThread();
	CloseTreatEventThread();
	CloseCheckParentThread();
    closedb();

    global_app_translators.clear();

	printf("Normal Exit alarmsrv!");

	return res;
}

