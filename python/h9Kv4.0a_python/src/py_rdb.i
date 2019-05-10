%module py_rdb
%{
#include    "../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../dps/v4.0a/inc/dps_type_def.h"
#include    "../../../dps/v4.0a/inc/ipc_common.h"
#include    "../../../dps/v4.0a/inc/dps_rec_def.h"
#include    "../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include    "../../../dps/v4.0a/inc/dps_functions_def.h"
#include    "../inc/py_rdb.h"
%}



%include "typemaps.i"
%apply double *OUTPUT { double *result };
%apply int              *OUTPUT { INTEGER *OUTPUT };
%apply float            *OUTPUT { FLOAT *OUTPUT };
%apply char             *OUTPUT { CHAR *OUTPUT };
%apply char             *OUTPUT { TINY *OUTPUT };
%apply double           *OUTPUT { REAL *OUTPUT };
%apply short            *OUTPUT { SHORT *OUTPUT };
%apply long             *OUTPUT { LONG *OUTPUT };
%apply int              *OUTPUT { GLONG *OUTPUT };
%apply unsigned int     *OUTPUT { GULONG *OUTPUT };
%apply unsigned short   *OUTPUT { USHORT *OUTPUT };
%apply unsigned long    *OUTPUT { BITS *OUTPUT };
%apply unsigned long    *OUTPUT { ULONG *OUTPUT };
%apply unsigned char    *OUTPUT { UTINY *OUTPUT };
%apply unsigned char    *OUTPUT { UCHAR *OUTPUT };


%apply double *INPUT { double *result };
%apply int              *INPUT { INTEGER *INPUT };
%apply float            *INPUT { FLOAT *INPUT };
%apply char             *INPUT { CHAR *INPUT };
%apply char             *INPUT { TINY *INPUT };
%apply double           *INPUT { REAL *INPUT };
%apply short            *INPUT { SHORT *INPUT };
%apply long             *INPUT { LONG *INPUT };
%apply int              *INPUT { GLONG *INPUT };
%apply unsigned int     *INPUT { GULONG *INPUT };
%apply unsigned short   *INPUT { USHORT *INPUT };
%apply unsigned long    *INPUT { BITS *INPUT };
%apply unsigned long    *INPUT { ULONG *INPUT };
%apply unsigned char    *INPUT { UTINY *INPUT };
%apply unsigned char    *INPUT { UCHAR *INPUT };



%apply double *INOUT { double *result };
%apply int              *INOUT { INTEGER *INOUT };
%apply float            *INOUT { FLOAT *INOUT };
%apply char             *INOUT { CHAR *INOUT };
%apply char             *INOUT { TINY *INOUT };
%apply double           *INOUT { REAL *INOUT };
%apply short            *INOUT { SHORT *INOUT };
%apply long             *INOUT { LONG *INOUT };
%apply int              *INOUT { GLONG *INOUT };
%apply unsigned int     *INOUT { GULONG *INOUT };
%apply unsigned short   *INOUT { USHORT *INOUT };
%apply unsigned long    *INOUT { BITS *INOUT };
%apply unsigned long    *INOUT { ULONG *INOUT };
%apply unsigned char    *INOUT { UTINY *INOUT };
%apply unsigned char    *INOUT { UCHAR *INOUT };

%inline %{
 
%}
typedef int	        INTEGER;
typedef float 	        FLOAT;
typedef unsigned int    UINT;    
typedef char		CHAR;
typedef char		TINY;
typedef double		REAL;
typedef short		SHORT;
typedef long		LONG;
typedef int		GLONG;		/* 32 bits all the time */
typedef unsigned int	GULONG;		/* 32 bits all the time */
typedef unsigned short	USHORT;
typedef unsigned long	BITS;
typedef unsigned long	ULONG;
typedef unsigned char	UTINY;
typedef unsigned char	UCHAR;


%include cstring.i
%cstring_bounded_output(char *outstring, 256);

extern int _readPointer(POINTER *INPUT) ;
extern int _connect_H9000_EMS(void);
extern int _disconnect_H9000_EMS(void);
extern int	_PyReadIntValByName(char *ptNameStr, int *value);
extern int	_PyReadFloatValByName(char *ptNameStr, float *OUTPUT);

/*    lan out cmd message       */
extern void Broadcast_sw(POINTER *pt, short stauts);
extern void Broadcast_ana(POINTER *pt, int value);
extern int  Broadcast_ana_lname(char * ptname, int value);
extern int Broadcast_sw_lname(char * ptname, short status);



extern int	_PyReadLogicNamelByID( POINTER *pt,char *outstring ) ;
/*link or unlink database*/
extern int	MountDB(char *INPUT);
extern int	UnMountDB(void);
extern int	MountDBFile(char *INPUT, BOOL INPUT);
extern void *MountFile(char *INPUT, GULONG INPUT);
extern int	UnMountFile(void* INPUT, GULONG INPUT);
 
/*Get database info*/
int GetDBName( char *outstring );
int GetDBLongname( char *outstring );
int GetDBVersion( char *outstring );
int GetStationNum( unsigned long  *OUTPUT   );


/*Get database point info*/
BOOL IsPointExist( POINTER *point );
int GetEntryNameById( POINTER *point, char *outstring );
int GetEntryLongnameById( POINTER *point, char *outstring );
int GetPtNameById( POINTER *point, char *ptNameStr, POINT_NAME *pt_name );
int GetPtLongnameById( POINTER *point, char *outstring );
int	GetPtCmdMsgSelById( POINTER *point, UCHAR *cmd_sel );
int	GetPtAlmMsgSelById( POINTER *point, UCHAR *alm_sel );
int	GetPtHardAddrById( POINTER *point, HARD_ADDR *hard_addr );
int	GetPtCtrlAddrById( POINTER *point, USHORT *ctrl_addr );
int	GetPtUpdateSrcById( POINTER *point, CHAR *update_src );
int	GetPtIoGroupById( POINTER *point, USHORT *io_group );
int	GetPtDspFileNameById( POINTER *point, char *outstring );
int	GetPtUpdateTimeById( POINTER *point, DATE_TIME *update_time );
int	IsPtValueManSetById( POINTER *point, BOOL *manSet );
int	IsPtValueChanFailById( POINTER *point, BOOL *chanFail);

int GetEntryIdByName( char *entryName, POINTER *point );
int GetEntryLongnameByNameStr( char *ptNameStr, char *entryLongname );
int GetPtLongnameByNameStr( char *ptNameStr, char *pointLongname );
int	GetPtCmdMsgSelByNameStr( char *ptNameStr, UCHAR *cmd_sel );
int	GetPtAlmMsgSelByNameStr( char *ptNameStr, UCHAR *alm_sel );
int	GetPtHardAddrByNameStr( char *ptNameStr, HARD_ADDR *hard_addr );
int	GetPtCtrlAddrByNameStr( char *ptNameStr, USHORT *ctrl_addr );
int	GetPtUpdateSrcByNameStr( char *ptNameStr, CHAR *update_src );
int	GetPtIoGroupByNameStr( char *ptNameStr, USHORT *io_group );
int	GetPtDspFileNameByNameStr( char *ptNameStr, char *dsp_filename );
int	GetPtUpdateTimeByNameStr( char *ptNameStr, DATE_TIME *update_time );
int	IsPtValueManSetByNameStr( char *ptNameStr, BOOL *manSet );
int	IsPtValueChanFailByNameStr( char *ptNameStr, BOOL *chanFail);

int GetPtCmdMsgStrById( POINTER *point, UCHAR cmdType, USHORT cmd_opt, CHAR *str );
int GetPtCmdMsgStrByNameStr( char *ptNameStr, UCHAR cmdType, USHORT cmd_opt, CHAR *str );

int GetPairPtById( POINTER *point, POINTER *pair_pt );
int GetPairPtByNameStr( char *ptNameStr, POINTER *pair_pt );

/*Get database entry info*/
int	GetPtIdByNameStr( char *ptNameStr, POINTER *point );
int	ReadEntryById( POINTER *point , DMS_COMMON *common_all);
int	ReadEntryByNameStr( char *ptNameStr, DMS_COMMON *common_all);
