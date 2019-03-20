/*********************************************************************************************
	�ļ����ƣ�	zconfigfileinfo.h
	��    �ݣ�	�����ļ���Ϣ��������(ZConfigFileInfo)����
	��    �ܣ�	��ȡ������Ϣ����ȡ��ʼ��������Ϣ����������������ж�
	��Ҫ������	1. GetInitObjectBuf()��������Ա����
                ���ܣ���ó�ʼ����ʷ�����Ӧ��ң�Ŷ���
				2. GetInitEntTypeStr()��������Ա����
                ���ܣ���ó�ʼ����ʷ�������������ַ���
				3. IsMeetCondition()��������Ա����
                ���ܣ��ж������Ƿ����㱣������
				4. GetConfigFileInfoObj()����̬��Ա����
                ���ܣ�������������
				5. ReadConfigFile()��˽�г�Ա����
                ���ܣ���ȡ�����ļ���Ϣ
				6. ReadInitObjectInfo()��˽�г�Ա����
                ���ܣ���ȡ��ʼ����ʷ�����Ӧ��ң�Ŷ���
				7. IsMeetEntType()��˽�г�Ա����
                ���ܣ��ж����������Ƿ����㱣������
				8. IsMeetEntObject()��˽�г�Ա����
                ���ܣ��ж���������Ƿ����㱣������
				9. GetRdbTableNameByObjectType()��˽�г�Ա����
                ���ܣ�ͨ���������ͻ��ʵʱ�����

	����˵����	��
	�޸���־��	��
*********************************************************************************************/

#ifndef _Z_CONFIG_FILE_INFO_H_
#define _Z_CONFIG_FILE_INFO_H_

#include "lendefine.h"
#include "netdef.h"

typedef struct 
{
	char			cObjCode[DEF_CODE_LEN];	//����
	int				iEntMode;	//��������ʽ
	unsigned char	ucValue;	//ֵ
}InitObjectInfoS;

//�����ʽ��I�ṹ
typedef struct
{
	int		index;			//��ʽ���
	char	desc[64];		//��ʽ����
	char	obj_type;		//��������
	char	alarm_mode;		//����ģʽ
	char	proc_mode;		//��������ģʽ
	int		time_delay;		//��ʱʱ��
	char	over_limit_mode;//һ��Խ�޷�ʽ
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