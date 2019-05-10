#ifndef	_STRING_TEXT_DEF
#define	_STRING_TEXT_DEF

#include	"../../../dms/v4.0a/inc/dms_com.h"

typedef struct 
{
	char		name[64];		//Ê¶±ð×Ö´®
	char		value[2][128];	//ÃèÊö
	USHORT		hash_index;
	USHORT		hash_next;
	GULONG		hash_key;
} STR_TEXT_UNIT; 


#define	MAX_STR_TEXT_UNIT_NUM	1111


typedef struct 
{
	UCHAR			ex_lang_state;
	USHORT			hash_pn;
	GULONG			def_num;
	STR_TEXT_UNIT	str_text_unit[MAX_STR_TEXT_UNIT_NUM];
} STR_TEXT_DEF;



#endif	/*_STRING_TEXT_DEF*/
