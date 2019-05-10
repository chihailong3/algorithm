%module py_data_type
%{
#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"
#include    "py_rdb.h"
%}


typedef	struct		/* Entry descriptor - fixed portion */
{
	CHAR		entryName[ENTRY_NAME_SIZE];	/* Entry's name	*/
	CHAR		entryLongname[ENTRY_LONGNAME_SIZE];	/* Entry's longname	*/
	CHAR		dsp_filename[DSP_FILENAME_SIZE];
    HARD_ADDR	hard_addr;
	USHORT		io_group;
	IOST		iost;
	IOATTR		ioattr;
	POINTER		pair_pt;
 	USHORT		ctrl_addr;
	MSG_SELECTOR msg_sel;
	UCHAR		update_src;
	USHORT		hash_index;
	USHORT		hash_next;
	GULONG		hash_key;
} ENTRY_FIXED;

typedef struct
{
	USHORT	viewPoint:3;
	USHORT	res1:2;
	USHORT	scanEnable:1;
	USHORT	almEnable:1;
	USHORT	almNoAck:1;
	USHORT	ctrlLock:1;
	USHORT	noUpdate:1;
	USHORT	almRepeat:1;
	USHORT	res2:1; //用于表示因闭锁延时后的事件记录
	USHORT	tagState:2;
	USHORT	res3:2;
} IOST;

typedef	struct
{
    POINTER    		point; 
 	ENTRY_FIXED		fixed;
	RD_VAR_UNION	var;
} DMS_COMMON;
 
enum	RW_ENG_LMT_OPT {RW_D_BAND,
						RW_HI_ALARM,
						RW_HI_WARN,
						RW_LO_WARN,
						RW_LO_ALARM,
						RW_HI_RECOVER,
						RW_LO_RECOVER,
						RW_ENG_CVT,
						RW_HI_RANGE,		
						RW_LO_RANGE,		
						RW_ZERO,		
						RW_COMPSATE,		
						RW_HI_CODE,		
						RW_LO_CODE,		
						RW_TR_WARN,
						RW_TR_RECOVER
						};

%{
typedef unsigned int ucharlong   ; 
%}

%typemap(in) ucharlong {
    $1 = PyInt_AsLong($input);
    if ($1 <0 || $1>255) 
    {
        PyErr_SetString(PyExc_ValueError, "UCHAR Can only assign to range from 0 to 255!");
        SWIG_fail;
    }
} 
 
%typemap(out) ucharlong {
$result = PyInt_FromLong($1);
}
 
 
typedef	struct
{
    ucharlong	stn_id;
	ucharlong	dev_id;         
	ucharlong	data_type;      
	ucharlong	update_src;
	int	pt_id;       
} POINTER;
%typemap(in) char ; 
%typemap(out) char ; 