TEMPLATE	= app
#LANGUAGE	= C++

QT += widgets
CONFIG	+= qt warn_off debug

win32:DEFINES	+= WIN32
INCLUDEPATH	+= . ./include  $${H9HOMEINCLUDE} $${H9HOME}/Debug/gui/tools/alarmsrv/ui

##########################################################################
#Ãÿ ‚∞Ê±æ 
#CONFIG  += baoji_ver
contains(CONFIG, baoji_ver) {
	DEFINES     +=  BAOJI_VERSION
}
##########################################################################

##########################################################################
#language translation 
#CONFIG  += nolangtr
contains(CONFIG, nolangtr) {
	DEFINES     +=  NO_SELF_LANG_TR
}
##########################################################################

contains(CONFIG, nolangtr) {
		win32:LIBS	+= -L$${H9HOME}/lib    -lh9cfg
		unix{
			##########################################################################
			#df8900 sun special version 
			#CONFIG  += df8900_sun
			contains(CONFIG, df8900_sun) {
				LIBS	+= -L$${H9HOME}/bin -L$${H9HOME}/lib -lqtdfnet -lqtdorbase -lqtexdb -lh9cfg
			}
			!contains(CONFIG, df8900_sun) {
				LIBS	+= -L$${H9HOME}/bin -L$${H9HOME}/lib    -lh9cfg
			}
			##########################################################################
		}
	}
!contains(CONFIG, nolangtr) {
		win32:LIBS	+= -L$${H9HOME}/lib    -lh9cfg 
		unix{
			##########################################################################
			#df8900 sun special version 
			#CONFIG  += df8900_sun
			contains(CONFIG, df8900_sun) {
				LIBS	+= -L$${H9HOME}/bin -L$${H9HOME}/lib -lqtdfnet -lqtdorbase -lqtexdb -lh9cfg 
			}
			!contains(CONFIG, df8900_sun) {
				LIBS	+= -L$${H9HOME}/bin -L$${H9HOME}/lib   -lh9cfg 
			}
			##########################################################################
		}
	}

HEADERS	+= 	include/alarmthread.h \
			include/inithiseventsprocessdlg.h \
			include/zconfigfileinfo.h
SOURCES	+= 	source/main.cxx \
			source/alarmthread.cxx \
			source/inithiseventsprocessdlg.cxx \
			source/zconfigfileinfo.cxx
FORMS	= 	ui/qinithiseventsprocessdlg.ui 

win32 {
DEFINES += WIN32 _AFXDLL
DEFINES -= _USRDLL
DESTDIR = $${H9HOME}/Debug/gui/tools/alarmsrv/obj
}

!exists( $${H9HOMEINCLUDE}/mmi/plat/Version_define.pri )
{
        !aix-g++:CONFIG += thread

        aix-xlc {
                QMAKE_LFLAGS += -brtl
                DEFINES += __unix _AIX
        }

        aix-g++{
                DEFINES += _AIX __unix
                QMAKE_LFLAGS += -Xlinker -brtl
        }
}

include($${H9HOMEINCLUDE}/mmi/plat/Version_define.pri)

TARGET = alarmsrv

MOC_DIR = $${H9HOME}/Debug/gui/tools/alarmsrv/moc
OBJECTS_DIR = $${H9HOME}/Debug/gui/tools/alarmsrv/obj
UI_DIR = $${H9HOME}/Debug/gui/tools/alarmsrv/ui

TRANSLATIONS = language/alarmsrv_en.ts language/alarmsrv_zh.ts
	
LIBPATH += \
	$${H9HOME}/lib
RC_FILE = alarmsrv.rc