#include "stdafx.h"
#include "secretstorage.h"
#include "../dcmapsys.h"
#include "g_blowfish.h"

DCMAP_REGISTER_STORAGEX(CSecretsDataStorage,"secrets",":Hidden:");

CSecretsDataStorage::CSecretsDataStorage()
{
	m_bCryptEnable = false;
	m_CustomHeaderSize = sizeof(m_CheckKey);
	m_BR.RegisterWideString("data",&value_type::data);

	m_iSessionStatus = 2;
	m_iSessionTime=60;

	memset(m_CheckKey,0,sizeof(m_CheckKey));
	memset(m_Key,0,sizeof(m_Key));
	m_iInUse = 0;
}

static void Encrypt(dcmapDSTR res,dcmapDCSTR data,int dlen,dcmapDCSTR key,int key_len=0)
{
	int data_len = dlen-1;

	int ps_pos = 0;
	int ps_len = dcslen(data)+1;
	int ps_mod=0;
	for(int i=0;i<data_len;i++,ps_pos++)
	{
		if(ps_pos >= ps_len)
		{
			ps_pos=0;
			ps_mod++;
		}
		res[i] = key[i]^data[ps_pos]; 
	}
	res[data_len]=0;
}

static void Decrypt(dcmapDSTR res,dcmapDCSTR data,int dlen,dcmapDCSTR key,int key_len=0)
{
	int data_len = dlen-1;

	for(int i=0;i<data_len;i++)
	{
		res[i] = data[i]^key[i]; 
	}
	res[data_len]=0;
}

static void MakeKey(dcmapDSTR key,int key_len,dcmapDCSTR pass)
{
	// fill key with password chars
	int ps_pos = 0;
	int ps_len = wcslen(pass);
	int isum=0;
	int ps_mod=0;
	for(int i=0;i<key_len;i++,ps_pos++)
	{
		if(ps_pos >= ps_len)
		{
			ps_pos=0;
			ps_mod++;
		}
		key[i] = pass[ps_pos]>>(ps_mod%5) - ps_mod*3;
		isum += key[i];
	}
	for(int i=0;i<key_len;i++)
	{
		int k = key_len-i-1;
		key[i] |= key[k]<<8;
		key[i] -= i+ps_len;
	}
	for(int i=0;i<key_len;i++)
	{
		int k = key_len-i-1;
		key[i] &= (key[k]&0x00ff << (key[i]%3+5))|0x00ff;
		key[i]-= (isum+k)%key[k];
		key[i]+=  (key[k]+i)%23;
	}
	// bitmess
	for(int i=0;i<key_len;i++)
	{
		int k = key_len-i-1;
		dcmapDCHAR& res = key[i];
		dcmapDCHAR rkey = i*257+isum+key[k]-key[i];
		res = rkey^res;
	}
}

bool CSecretsDataStorage::ProcessLoadData(value_type* pdata)
{
	/*
	if(pdata->enc)
	{
		if(m_MasterPassword.empty())
		{
			dcmapDCSTR pass = m_pCore->GetCoreUI()->InputBox(dcmapUT("Enter password"));
			m_MasterPassword = pass?pass:dcmapDT("");
			if(MakeKey())
			{
				Decrypt(pdata);
			}
		}
		else
		{
			Decrypt(pdata);
		}
	}
	*/
	return standart_storage::ProcessLoadData(pdata);
}

bool CSecretsDataStorage::DoLoadData()
{
	return standart_storage::DoLoadData();
}

dcmapCSTR DCMAPAPI CSecretsDataStorage::GetName()
{
	static string name;
	name = StrD2C(m_pCurrentRecord->data);
	return name.c_str();
}

dcmapDCSTR DCMAPAPI CSecretsDataStorage::GetWideName()
{
	return m_pCurrentRecord->data;
}

void DCMAPAPI CSecretsDataStorage::SetName( const char* szName )
{
	SetWideName(ToDStr(szName).c_str());
}

void DCMAPAPI CSecretsDataStorage::SetWideName( dcmapDCSTR szName )
{
	dcsncpy(m_temp.data,szName,value_type::data_len);
	m_temp.data[value_type::data_len-1]=0;
}

int DCMAPAPI CSecretsDataStorage::TableParams()
{
	return DCMAP_TABLE_INTERNAL | DCMAP_TABLE_HIDDEN | Inherited::TableParams();
}

bool DCMAPAPI CSecretsDataStorage::OpenSession(dcmapWCSTR password)
{
	if(!DataLoad())return false;
	if(m_iSessionStatus>0)
	{
		m_iInUse++;
		m_OpenTime = dcmapGetTime();
		return true;
	}
	if(!password || !password[0])
	{
		while(true)
		{
			password = m_pCore->GetCoreUI()->InputPassword(LSTRW(EnterPasswordMsg));
			if(!password)return false;
			if(!CheckPassword(password))
			{
				m_pCore->GetCoreUI()->ErrorMsg(LSTRW(WrongPasswordMsg));
			}
			else break;
		}
	}
	else
	{
		if(!CheckPassword(password))return false;
	}

	::MakeKey(m_Key,value_type::data_len,password);

	// iterate storage & decrypt
	if(1)
	{
		_foreach(DataList,m_lstData,it) // Set new loaded index
		{
			if(it->enc==1)
			{
				::Decrypt(it->data,it->data,value_type::data_len,m_Key);
			}
			it->enc = 0;
		}
		m_iSessionStatus=1;
	}
	m_iInUse++;
	m_OpenTime = dcmapGetTime();
	return true;
}

void DCMAPAPI CSecretsDataStorage::EndSession()
{
	if(m_iInUse>0)m_iInUse--;
	if(!m_iInUse)
	{
		int dt = dcmapGetTime()-m_OpenTime;
		if(dt > m_iSessionTime)
		{
			CloseSession();
		}
	}
}

void DCMAPAPI CSecretsDataStorage::CloseSession()
{
	if(!DataLoad())return;
	if(m_iSessionStatus==2)return;

	if(m_iSessionStatus==1) // iterate storage & encrypt
	{
		_foreach(DataList,m_lstData,it) // Set new loaded index
		{
			if(!it->enc)
			{
				::Encrypt(it->data,it->data,value_type::data_len,m_Key);
				it->enc = 1;
			}
		}
		m_iSessionStatus=0;
		memset(m_Key,0,sizeof(m_Key));
	}
	m_iInUse=0;
}

wstring MakeCheckStr(dcmapWCSTR pass){return wstring(L"DCMapSecrets_")+pass+L"_DCMapSecrets";}

bool DCMAPAPI CSecretsDataStorage::CheckPassword(dcmapWCSTR pass)
{
	if(!DataLoad())return false;
	if(!pass)return false;
	if(m_iSessionStatus == 2) return pass[0]==0;


	dcmapDCHAR ResCheck[value_type::data_len];
	dcmapDCHAR ResKey[value_type::data_len];
	dcmapDCHAR ResCmp[value_type::data_len];
	::MakeKey(ResCheck,value_type::data_len,MakeCheckStr(pass).c_str());
	::MakeKey(ResKey,value_type::data_len,pass);

	::Decrypt(ResCmp,m_CheckKey,value_type::data_len,ResKey);

	return memcmp(ResCmp,ResCheck,value_type::data_len)==0;
}


bool DCMAPAPI CSecretsDataStorage::ChangePassword(dcmapWCSTR oldp,dcmapWCSTR newp)
{
	if(!DataLoad())return false;
	if(!oldp || !newp)return false;


	if(m_iSessionStatus == 0) // open session first
	{
		if(!OpenSession(oldp))return false;
	}
	else if(m_iSessionStatus == 1) // only check password
	{
		if(!CheckPassword(oldp))return false;
	}

	if(!newp || !newp[0]) // remove password
	{
		m_modIndex++; // enforce for save
		m_iSessionStatus = 2;
		return true;
	}

	// make check key
	::MakeKey(m_CheckKey,value_type::data_len,MakeCheckStr(newp).c_str());

	// make encode key
	::MakeKey(m_Key,value_type::data_len,newp);

	// encode check
	::Encrypt(m_CheckKey,m_CheckKey,value_type::data_len,m_Key);

	m_modIndex++; // enforce for save
	m_iSessionStatus = 1;

	CloseSession();
	return true;
}

int DCMAPAPI CSecretsDataStorage::SessionStatus()
{
	return m_iSessionStatus;
}

void CSecretsDataStorage::ProcessCustomHeader( dcmapSHORT cbSize,dcmapBYTE const* buf )
{
	if(cbSize == sizeof(m_CheckKey))
	{
		memcpy(m_CheckKey,buf,cbSize);
		m_iSessionStatus = 0;
	}
}

bool CSecretsDataStorage::GetCustomHeader( dcmapSHORT &cbSize,dcmapBYTE const* &buf )
{
	if(	m_iSessionStatus == 2)return false;
	CloseSession();

	cbSize = sizeof(m_CheckKey);
	buf = (dcmapBYTE const*)m_CheckKey;
	return true;
}

bool CSecretsDataStorage::ProcessUpdatedData(value_type* data)
{
	if(!OpenSession())return false;
	data->enc = 0;
	return true;
}

bool CSecretsDataStorage::ProcessNewData(value_type* data)
{
	if(!OpenSession())return false;
	data->enc = 0;
	return true;
}

void CSecretsDataStorage::OnChange()
{
	EndSession();
};

bool CSecretsDataStorage::OnDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
{
	return true;
}

void  DCMAPAPI CSecretsDataStorage::LoadConfig(IDCMapConfig* pConfig)
{
	LoadPrefs();
}

void CSecretsDataStorage::LoadPrefs()
{
	DCMapConfigWrapper cfg(m_pCore);
	cfg.section("Secrets");
	m_iSessionTime = 1;
	cfg["RememberPasswordTime"] >> m_iSessionTime;
	m_iSessionTime *= 60;
}


dcmapLRESULT DCMAPAPI CSecretsDataStorage::OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_CONFIG_CHANGED:  // close session if it is time
		LoadPrefs();
		break;
	case DCMM_SYSTEM_TIMER_TICK:  // close session if it is time
		if(m_iSessionStatus == 1 && !m_iInUse)
		{
			int dt = dcmapGetTime()-m_OpenTime;
			if(dt > m_iSessionTime)
			{
				CloseSession();
			}
		}
		break;
	}
	return Inherited::OnMessage(Msg,wParam,lParam,pSender);
}


int DCMAPAPI CSecretsDataStorage::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

	if(wcsicmp(command,L"OpenSession")==0)
	{
		int n = pParams->Count();
		dcmapWCSTR password = pParams->GetString();
		int r = OpenSession(n?password:0);
		pResult->AddNumber(r);
		return DCMEXEC_SUCCESS;
	}
	else if(wcsicmp(command,L"CloseSession")==0)
	{
		CloseSession();
		return DCMEXEC_SUCCESS;
	}
	else if(wcsicmp(command,L"EndSession")==0)
	{
		EndSession();
		return DCMEXEC_SUCCESS;
	}
	else if(wcsicmp(command,L"ChangePassword")==0)
	{
		dcmapWCSTR old_password = pParams->GetString(0);
		dcmapWCSTR new_password = pParams->GetString(1);
		int r = ChangePassword(old_password,new_password);
		pResult->AddNumber(r);
		return DCMEXEC_SUCCESS;
	}
	return DCMEXEC_FAIL;
}