#include "stdafx.h"
#include "database.h"
#include "utils.h"
#include "../api/dcmap_script_helper.h"


DCMAP_SCRIPT_REFLECTION(CDCMapDatabase)
{
	DCMAP_SCRIPT_FUNC(ListTables);
}

CDCMapDatabase::CDCMapDatabase():
	m_pCore(0),
	m_pSys(0),
	m_iDataUpdateLock(0),
	m_bNeedUpdate(false)
{

}

bool DCMAPAPI CDCMapDatabase::Load( dcmapFCSTR szLocation,dcmapFCSTR szTemplateLocation,int Flags,int LoadFlags)
{
	IDCMapCoreUI* pCoreUI=0;
	if(m_pCore)pCoreUI = m_pCore->GetCoreUI();

	if(!szLocation)return false;

	_foreach(MapStorages,m_mapStorages,it)
	{
		sysstring FileName = szLocation+ToSys(it->first.c_str())+dcmapFT(".db");
		sysstring FileNameZ = szLocation+ToSys(it->first.c_str())+dcmapFT(".db.gz");

		if(dcmapFileExists(FileName.c_str()))
		{
			if(!it->second->LoadDataFromFile(FileName.c_str(),LoadFlags))
			{
				if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableLoadError))%ToWide(it->first)).c_str());
			}
			else continue;
		}
		else if(dcmapFileExists(FileNameZ.c_str()))
		{
			if(!it->second->LoadDataFromFile(FileNameZ.c_str(),LoadFlags|DCMAP_DSLF_GZIPPED))
			{
				if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableLoadError))%ToWide(it->first)).c_str());
			}
			else continue;
		}
		else if(szTemplateLocation)
		{
			FileName = szTemplateLocation+ToSys(it->first.c_str())+dcmapFT(".db");
			if(dcmapFileExists(FileName.c_str()))
			{
				if(!it->second->LoadDataFromFile(FileName.c_str(),LoadFlags))
				{
					if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableLoadError))%ToWide(it->first)).c_str());
				}
				else continue;
			}
		}
		it->second->LoadEmpty();
	}
	return true;
}

bool DCMAPAPI CDCMapDatabase::Save(dcmapFCSTR szLocation,int Flags, int SaveFlags )
{
	IDCMapCoreUI* pCoreUI=0;
	if(m_pCore)pCoreUI = m_pCore->GetCoreUI();

	_foreach(MapStorages,m_mapStorages,it)
	{
		if(!(Flags & DCMAP_DBSLF_SAVE_ONLY_MODIFIED) || it->second->Changed() )
		{
			fstring fileb = szLocation+ToSys(it->first.c_str());
			fstring filen,fileo;

			if(SaveFlags & DCMAP_DSLF_GZIPPED)
			{
				filen = fileb+dcmapFT(".db.gz");
				fileo = fileb+dcmapFT(".db");
			}
			else 
			{
				filen = fileb+dcmapFT(".db");
				fileo = fileb+dcmapFT(".db.gz");
			}

			fstring filet = filen+dcmapFT(".temp");

			dcmapDeleteFile(fileo.c_str());

			if(dcmapFileExists(filen.c_str()))
			{
				if(!it->second->SaveDataToFile(filet.c_str(),SaveFlags))
				{
					if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableSaveError))%ToWide(it->first)).c_str());
					dcmapDeleteFile(filet.c_str());
				}
				else
				{
					if(!dcmapDeleteFile(filen.c_str()) || !dcmapRename(filet.c_str(),filen.c_str()))
					{
						if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableSaveError))%ToWide(it->first)).c_str());
						dcmapDeleteFile(filet.c_str());
					}
				}
			}
			else
			{
				if(!it->second->SaveDataToFile(filen.c_str(),SaveFlags))
				{
					if(pCoreUI)pCoreUI->ErrorMsg(boost::str(wformat(LSTRW(TableSaveError))%ToWide(it->first)).c_str());
					dcmapDeleteFile(filen.c_str());
				}
			}
		}
	}
	return true;
}

bool DCMAPAPI CDCMapDatabase::DataChanged()
{
	bool need_save = false; // Test, if we got something to save
	_foreach(MapStorages,m_mapStorages,it)
	{
		need_save |= it->second->Changed();
	}
	return need_save;
}

bool DCMAPAPI CDCMapDatabase::SaveWithBackup( dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int iNumBackups,int Flags, int SaveFlags)
{
	sysstring Dir =  pDir?pDir:GetAppStartupDir()+dcmapFT("data\\");
	sysstring Backup = pBackupDir?pBackupDir:GetAppStartupDir()+dcmapFT("backup\\");

	dcmapMakeDir(Dir.c_str());

	if(iNumBackups)
	{
		// first rotate backup dirs
		sysstring base = Backup;
		//CreateDirectory()

		dcmapDWORD MinBackup=0xffffffff;
		dcmapDWORD MaxBackup=0;
		dcmapDWORD BackupCount=0;
		VecFiles dirs;
		ListDirectory((base+dcmapFT("bk*")).c_str(),dirs);
		_foreach(VecFiles,dirs,dir)
		{
			dcmapDWORD n;
			if(dir->size() <= 2)continue;
			if(gsys::lex_convert(n,dir->c_str()+2))
			{
				MinBackup = min(n,MinBackup);
				MaxBackup = max(n,MaxBackup);
				BackupCount++;
			}
		}
		while(BackupCount > iNumBackups)
		{
			DestroyDir((base +dcmapFT("bk")+ ToSys(gsys::to_str(MinBackup).c_str())).c_str());
			++MinBackup;
			--BackupCount;
		}
		base = base+dcmapFT("bk")+ToSys(gsys::to_str(MaxBackup+1).c_str())+dcmapFT("\\");
		dcmapMakeDir(base.c_str());

		if(!(Flags & DCMAP_DBSLF_USE_HARDLINKS)) // move files to speedup copy
		{
			_foreach(MapStorages,m_mapStorages,it)
			{
				sysstring oldname = Dir+ToSys(it->first.c_str());
				sysstring newname = base+ToSys(it->first.c_str());
				if(it->second->Changed())
				{
					dcmapRename((oldname+dcmapFT(".db")).c_str(),(newname+dcmapFT(".db")).c_str());
					dcmapRename((oldname+dcmapFT(".db.gz")).c_str(),(newname+dcmapFT(".db.gz")).c_str());
				}
			}
		}
		sysstring fdir = (sysstring)Dir+dcmapFT("*.db*");

		VecFiles files;
		ListDirectory(fdir.c_str(),files);
		_foreach(VecFiles,files,file)
		{
			sysstring oldname = (sysstring)Dir+*file;
			sysstring newname = base+*file;
			if(Flags & DCMAP_DBSLF_USE_HARDLINKS)
				dcmapHardLinkFile(oldname.c_str(),newname.c_str(),false);	
			else
				dcmapCopyFile(oldname.c_str(),newname.c_str(),false);
		}
	}
	return Save(Dir.c_str(),Flags,SaveFlags);
}

bool DCMAPAPI CDCMapDatabase::LoadEmpty()
{
	_foreach(MapStorages,m_mapStorages,it)
	{
		it->second->LoadEmpty();
	}
	return true;
}

IDCMapDataStorage* DCMAPAPI CDCMapDatabase::GetDataStorage( dcmapCSTR pName )
{
	MapStorages::iterator it = m_mapStorages.find(pName);
	if(it == m_mapStorages.end())
	{
		// find alias
		pName = g_pDCMapSys->GetPluginNameByAlias("PDataStorage",pName);
		it = m_mapStorages.find(pName);
		if(it == m_mapStorages.end())return 0;
	}
	return it->second;
}

IDCMapDataSource* DCMAPAPI CDCMapDatabase::GetDataSource( dcmapCSTR pName )
{
	return GetDataStorage(pName);
}

bool  DCMAPAPI CDCMapDatabase::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pCore = core;
	LoadStorageModules("PDataStorage");
	PostInit();
	return true;
}

dcmapLRESULT DCMAPAPI CDCMapDatabase::OnMessage( dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender )
{

	return 0;
}

void CDCMapDatabase::LoadStorageModules(dcmapCSTR Classname)
{
	std::vector<string> strs;
	g_pDCMapSys->GetClassPluginNamesT(Classname,strs);

	for(int i=0;i<strs.size();i++)
	{
		IDCMapDataStorage* pStor = 0;
		
		if(m_pCore) pStor = m_pCore->CreateModuleTyped<IDCMapDataStorage>(Classname,strs[i].c_str());
		else pStor = m_pSys->CreateObject(Classname,strs[i].c_str())->Cast<IDCMapDataStorage>();

		if(pStor)
		{
			if(pStor->InitStorage(this))
			{
				m_mapStorages[strs[i]] = pStor;
			}
			else
			{
				if(m_pCore) m_pCore->DestroyModule(pStor);
				else m_pSys->DestroyObject(pStor);
			}
		}
	}
}

void DCMAPAPI CDCMapDatabase::PostInit()
{
	_foreach(MapStorages,m_mapStorages,it)
	{
		it->second->PostInit();
	}
}

bool  DCMAPAPI CDCMapDatabase::OnCreate( DCMapObjectCreationParams const* params )
{
	m_pSys = params->pDCMapSys;
	return true;
}

void DCMAPAPI CDCMapDatabase::OnDataUpdate()
{
	if(m_iDataUpdateLock <= 0)
	{
		if(m_pCore)m_pCore->OnDataUpdate();
	}
	else
	{
		m_bNeedUpdate=true;
	}
}

void DCMAPAPI CDCMapDatabase::LockDataUpdate()
{
	m_iDataUpdateLock++;
}
void DCMAPAPI CDCMapDatabase::UnlockDataUpdate()
{
	m_iDataUpdateLock--;
	if(m_iDataUpdateLock<=0)
	{
		m_iDataUpdateLock=0;
		if(m_bNeedUpdate)OnDataUpdate();
	}
}

void* DCMAPAPI CDCMapDatabase::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapDatabaseEx);
	DCMAP_MODULE_SUPPORTS(IDCMapDatabase);
	DCMAP_MODULE_SUPPORTS(IDCMapModule);
	DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);
	return 0;
}

IDCMapVarValue* DCMAPAPI CDCMapDatabase::ListTables()
{
	IDCMapVarValuePtr lst(m_pCore->CreateVarValue());

	_foreach(MapStorages,m_mapStorages,it)
	{
		lst[ToWStr(it->first)] = it->second;
	}
	return lst;
}