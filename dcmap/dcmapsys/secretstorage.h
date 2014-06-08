#pragma once
#include "../dcmapsys.h"

#include "BaseDataStorage.h"

class CSecretsDataStorage :	public CStandartDataStorage<DCMapSecretsInfo,IDCMapSecretsDataStorage,CSecretsDataStorage>
{
	typedef CStandartDataStorage<DCMapSecretsInfo,IDCMapSecretsDataStorage,CSecretsDataStorage> Inherited;
protected:
	virtual bool DoLoadData();

	dcmapDCHAR m_Key[value_type::data_len];
	dcmapDCHAR m_CheckKey[value_type::data_len];

	bool m_bCryptEnable;
public:
	CSecretsDataStorage();

	dcmapCSTR DCMAPAPI Name(){return "secrets";}

	dcmapCSTR DCMAPAPI GetName();
	dcmapDCSTR DCMAPAPI GetWideName();
	void DCMAPAPI SetName(const char* szName);
	void DCMAPAPI SetWideName(dcmapDCSTR szName);
	bool ProcessLoadData(value_type* pdata);

	void ProcessCustomHeader(dcmapSHORT cbSize,dcmapBYTE const* buf);
	bool GetCustomHeader(dcmapSHORT &cbSize,dcmapBYTE const* &buf);

	int m_iSessionStatus;
	int m_iInUse;
	dcmapTIME m_OpenTime;

	int m_iSessionTime;

public:
	int DCMAPAPI TableParams();

	bool DCMAPAPI OpenSession(dcmapWCSTR password=0);
	void DCMAPAPI CloseSession();
	int DCMAPAPI SessionStatus();
	bool DCMAPAPI ChangePassword(dcmapWCSTR oldp,dcmapWCSTR newp);
	bool DCMAPAPI CheckPassword(dcmapWCSTR pass);

	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool ProcessUpdatedData(value_type* data);
	bool ProcessNewData(value_type* data);
	bool OnDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res);
	void OnChange();
	void DCMAPAPI EndSession();
	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
	void LoadPrefs();
	void DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
};