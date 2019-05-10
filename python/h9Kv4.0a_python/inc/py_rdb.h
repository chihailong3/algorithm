#ifndef  _PY_RDB_H__
#define  _PY_RDB_H__


/////////////////////////H head file defined here////////////////////////
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include    "../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"



extern int _connect_H9000_EMS(void);
extern int _disconnect_H9000_EMS(void);
extern int	_PyReadIntValByName(char *ptNameStr, int *value);
extern int	_PyReadFloatValByName(char *ptNameStr, float *value);



extern	SHM_RECORD *shm_rec_addr;
extern void Broadcast_sw(POINTER *pt, short stauts);
extern void Broadcast_ana(POINTER *pt, int value);
extern int  Broadcast_ana_lname(char * ptname, int value);
extern int Broadcast_sw_lname(char * ptname, short status);
////////////////////////end head file ////////////////////////////


#endif