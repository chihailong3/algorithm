/*********************************************************************************************
	文件名称：	zconfigfileinfo.h
	内    容：	配置文件信息和条件类(ZConfigFileInfo)声明
	功    能：	读取配置信息，读取初始化对象信息，处理事项保存条件判断
	主要函数：	1. GetInitObjectBuf()：公共成员函数
                功能：获得初始化历史事项对应的遥信对象
				2. GetInitEntTypeStr()：公共成员函数
                功能：获得初始化历史事项类型条件字符串
				3. IsMeetCondition()：公共成员函数
                功能：判断事项是否满足保存条件
				4. GetConfigFileInfoObj()：静态成员函数
                功能：获得配置类对象
				5. ReadConfigFile()：私有成员函数
                功能：读取配置文件信息
				6. ReadInitObjectInfo()：私有成员函数
                功能：读取初始化历史事项对应的遥信对象
				7. IsMeetEntType()：私有成员函数
                功能：判断事项类型是否满足保存条件
				8. IsMeetEntObject()：私有成员函数
                功能：判断事项对象是否满足保存条件
				9. GetRdbTableNameByObjectType()：私有成员函数
                功能：通过对象类型获得实时库表名

	其它说明：	无
	修改日志：	无
*********************************************************************************************/

#ifndef _Z_CONFIG_FILE_INFO_H_
#define _Z_CONFIG_FILE_INFO_H_

#include "lendefine.h"
#include "netdef.h"

typedef struct 
{
	char			cObjCode[DEF_CODE_LEN];	//代码
	int				iEntMode;	//报警处理方式
	unsigned char	ucValue;	//值
}InitObjectInfoS;

//事项处理方式表I结构
typedef struct
{
	int		index;			//方式序号
	char	desc[64];		//方式描述
	char	obj_type;		//对象类型
	char	alarm_mode;		//报警模式
	char	proc_mode;		//报警处理模式
	int		time_delay;		//延时时间
	char	over_limit_mode;//一级越限方式
}EntProcModeData;

class ZConfigFileInfo
{
	char	m_acInitEntTypeStr[512];

	bool	m_bReloadProcModeParaF;

	int		m_iObjectTypeNum;
	int*	m_pObjectTypeBuf;
	int		m_iEventTypeNum;
	int*	m_pEventTypeBuf;
	int		m_iEventProcModeNum;
	EntProcModeData*	m_pEventProcModeBuf;

	int		m_iInitObjectNum;
	InitObjectInfoS* m_pInitObjectBuf;

	void ReadConfigFile();
	void ReadInitObjectInfo();

	bool IsMeetEntType(int iEntType);
	bool IsMeetEntObject(SYS_EVENT* pBuf);

	const char* GetRdbTableNameByObjectType(int iObjType);

	bool IsProcModeMeetCondition(const int & procmode, const int & state);
	void GetEntProcModePara();

public:
    ZConfigFileInfo( );
    ~ZConfigFileInfo( );

	InitObjectInfoS* GetInitObjectBuf(int& iInitObjNum, bool bReloadF = false);
	char* GetInitEntTypeStr();
	bool IsMeetCondition(SYS_EVENT* pBuf);
	void SetReloadEntProcModePara() { m_bReloadProcModeParaF = true;}

	static ZConfigFileInfo* GetConfigFileInfoObj();
};


#endif//_Z_CONFIG_FILE_INFO_H_