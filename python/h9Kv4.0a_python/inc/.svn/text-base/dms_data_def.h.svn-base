
#ifndef	_DMS_DATA_DEF_H
#define	_DMS_DATA_DEF_H

#include	"./os_type_def.h"
#include	"./dms_structure_def.h"



#define   MAX_DATATYPE_NUM	16
enum    DATA_TYPE  { ANA_TYPE=1, IMP_TYPE=3, SOE_TYPE=4, POL_TYPE=5, 
			SYSMSG_TYPE=6, OUTA_TYPE=7, OUTS_TYPE=8, 
			CALC_TYPE=9, CURVE_TYPE=10, PARAM_TYPE=12, 
			CHARA_TYPE=13, DEVOBJ_TYPE=14, TIME_TYPE=15};


static	char *recordname[] = 
				 {(char *)"",      (char *)"ANA",   (char *)"",     (char *)"IMP",  (char *)"INT", 
				 (char *)"POL",   (char *)"SYS",(char *)"OUA", (char *)"OUS", (char *)"CAL", 
				 (char *)"CUR", (char *)"", (char *)"PAM",     (char *)"CHA", (char *)"OBJ",
					(char *)"TME"};

static	char *recordlongname[] = 
				 {(char *)"",      (char *)"模拟量",   (char *)"",     (char *)"脉冲量",  (char *)"中断开入量", 
				 (char *)"扫查开入量",   (char *)"综合信息量",(char *)"模拟输出量", (char *)"开关输出量", (char *)"计算量", 
				 (char *)"曲线点", (char *)"", (char *)"整定值",     (char *)"温度特征值", (char *)"设备对象",
					(char *)"时间点"};

static	char *recordExlongname[] = 
				 {(char *)"",      (char *)"Measurand",   (char *)"",     (char *)"Impulse",  (char *)"SOE", 
				 (char *)"Indication",   (char *)"Infomation",(char *)"Output Measurand", (char *)"Output Indication", (char *)"Calcu Measurand", 
				 (char *)"Curve", (char *)"", (char *)"Parameter",     (char *)"Temp Character", (char *)"Object",
				 (char *)"Time"};





/*
define database read/write common data structure
*/
typedef union
{
	ENTRY_STAT_VAL	ind;
	ENTRY_ANLG_VAL	anlg;
	struct
	{
		ENTRY_IMP_VAL	entry;
		IMP_FLOAT_DATA	init;
		IMP_FLOAT_DATA	last;
		IMP_FLOAT_DATA	peak;
		IMP_FLOAT_DATA	valley;
		IMP_FLOAT_DATA	normal;			
		IMP_FLOAT_DATA	sum;			
		IMP_FLOAT_DATA	ppeak;			
	} imp;
	ENTRY_CHARA_VAL	chara;
	ENTRY_ANLG_VAL	calc;
	ENTRY_CURVE_VAL	curve;
	ENTRY_DEVOBJ_VAL	devobj;
	ENTRY_TIME_VAL	time;
} RD_VAR_UNION;


typedef	struct
{
    POINTER    		point; 
 	ENTRY_FIXED		fixed;
	RD_VAR_UNION	var;
} DMS_COMMON;


typedef	struct
{
    POINTER    		point; 
 	ENTRY_FIXED		*fixed_ptr;
	RD_VAR_UNION	*var_ptr;
} DMS_COMMON_PTR;


typedef	union
{
	IND_DATA		ind;
	ANLG_DATA		anlg;
	ANLG_DATA		calc;
	struct
	{
		IMP_STATUS	status;
		GULONG		iValue;
		FLOAT		fValue;
	} imp;
} WR_VAR_UNION;



typedef struct
{
	char	stnName[STATION_NAME_SIZE];
	char	grpName[GROUP_NAME_SIZE];
	char	recName[RECORD_NAME_SIZE];
	char	entryName[ENTRY_NAME_SIZE];
} POINT_NAME;	




enum	RW_FLAG			{	RW_UPDATE_TIME		= 1<<0, 
							RW_RT_VALUE			= 1<<1,
							RW_VIEWPOINT		= 1<<2,
							RW_SCAN_ENABLE		= 1<<3,
							RW_ALM_ENABLE		= 1<<4,
							RW_ALM_NOACK		= 1<<5,
							RW_CTRL_LOCK		= 1<<6,
							RW_UPDATED			= 1<<7,
							RW_ALM_REPEAT		= 1<<8,
							RW_TAG_STATE		= 1<<9,
							RW_CHANGE_TIME		= 1<<10,
							RW_ADD_NORMAL_OPEN	= 1<<11,
							RW_ADD_NORMAL_CLOSE	= 1<<12,
							RW_ADD_FAULT_OPEN	= 1<<13,
							RW_ADD_FAULT_CLOSE	= 1<<14,
							RW_FAULT_TAG		= 1<<15,
							RW_MAN_SET			= 1<<16,
							RW_CTRL_BLOCK		= 1<<16,/*same as MAN_SET*/	
							RW_CHAN_FAIL		= 1<<17,
							RW_LMT_STATE		= 1<<18,
							RW_TREND_STATE		= 1<<19,
							RW_OVERFLOW			= 1<<20
						};


enum	RW_IOATTR_FLAG {	RW_PROCTYPE		= 1<<0, 
							RW_ALM_CBA		= 1<<1,	
							RW_STATISTIC	= 1<<2,
							RW_FAULT		= 1<<3,
							RW_FAIL			= 1<<4,
							RW_CTRL			= 1<<5,
							RW_SOE			= 1<<6,
							RW_TREND_ALM	= 1<<7,	
							RW_KEY_SIGNAL	= 1<<8,
							RW_FAULT_REC	= 1<<9,
							RW_AUD_ALM		= 1<<10,
							RW_NORM_CLOSE	= 1<<11,
							RW_DOUBLE_PT	= 1<<12,
							RW_SYS_VIRTUAL	= 1<<13,
							RW_LCU_VIRTUAL	= 1<<14,
							RW_CASCADE		= 1<<15,
							RW_HDB_REC		= 1<<16,
							RW_HDB_CURVE	= 1<<17,
							RW_ON_ALM		= 1<<18,
							RW_OFF_ALM		= 1<<19,
							RW_HDB_PERIOD	= 1<<20,
							RW_NORM_STATE	= 1<<21
						};

/*下面的RW_IMP_OPT定义不可修改，必须从零值开始，顺序不能改变，以对应于status定义*/
enum	RW_IMP_OPT {RW_IMP_ACCUM =0,
					RW_D_ACCUM,  
					RW_D_DAY_PEAK,
					RW_D_MONTH_PEAK,
					RW_D_YEAR_PEAK,
					RW_D_DAY_VALLY,
					RW_D_MONTH_VALLY,
					RW_D_YEAR_VALLY,
					RW_D_DAY_NORMAL,
					RW_D_MONTH_NORMAL,
					RW_D_YEAR_NORMAL,
					RW_D_DAY_SUM,
					RW_D_MONTH_SUM,
					RW_D_YEAR_SUM,
					RW_D_DAY_PPEAK,
					RW_D_MONTH_PPEAK,
					RW_D_YEAR_PPEAK	
				};


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


enum	RW_STC_OPT	{RW_STC_NORMAL_OPEN,
					RW_STC_NORMAL_CLOSE,
					RW_STC_FAULT_OPEN,
					RW_STC_FAULT_CLOSE
					};



enum	RD_IMP_DATA_OPT	{RD_IMP_DATA_PPEAK,
						RD_IMP_DATA_PEAK,
						RD_IMP_DATA_VALLEY,
						RD_IMP_DATA_NORMAL,
						RD_IMP_DATA_SUM,
						RD_IMP_DATA_INIT,
						RD_IMP_DATA_LAST
						};

enum	IMP_STATISTIC_TYPE	{RD_IMP_DAY,
						RD_IMP_MONTH,
						RD_IMP_YEAR
						};





#endif   /*_DMS_DATA_DEF_H*/


