// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"
#include "utils.h"

#include "parceutil.h"

#include <g_text.h>
#include <strstream>

#include <string>

using namespace std;
using namespace gsys;

class CCSVTableImport : public IDCMapImportProcessor
{
public:
	wstring m_strFormat;

	typedef std::vector<std::wstring> VecRecords;
	typedef std::vector<wchar_t> VecDelims;
	typedef vector<int> VecIndexes;
	typedef vector<bool> VecMask;
	typedef gsys::strimap<string,int> MapIndexes;

	typedef vector<IDCMapDataStorage*> VecRelations;
	VecRelations TableRel;

	VecDelims m_Delims;
	IDCMapCore* m_pCore;

	VecIndexes ind;

	VecIndexes keyind;

	//MapIndexes mapInd;

	IDCMapDataStorage* m_pData;

	void DCMAPAPI Destroy(){delete this;}

	CCSVTableImport()
	{
		num_insert=0;
	}

	struct inf_rec
	{
		wstring str;
		dcmapREAL num;
		bool is_str;
	};
	typedef vector<inf_rec> PInf;

	typedef vector<PInf*> VecItems;
	VecItems m_vecItems;

	virtual bool CheckIndexes(){return true;}

	int SetupIndexes(VecRecords &rec)
	{
		for(int i=0;i<ind.size();i++)ind[i]=-1;
		for(int i=0;i<rec.size();i++)
		{
            string coln = WideToString(rec[i]);
            if(coln.data()) strlwr((char*)coln.data()); 
			int id = m_pData->ColumnID(coln.c_str());
			if(id >= 0) ind[id] = i;
		}
		if(!CheckIndexes())return 0;

		int num=0;
		for(int i=0;i<ind.size();i++)
		{
			if(ind[i]>=0)num++;
		}
		return num;
	}

	bool QueryIndexes(wstring const& line)
	{
		DCMapConfigWrapper cfg(m_pCore->GetConfig());
		cfg.section(GetConfigSectionName());

		int NumParams=0;
		cfg["Params"] >> NumParams;

		int max_read=-1;
		int best_ind=-1;
		wstring BestParam;// = c_szDefaultFormat;

		for(int i=0;i<NumParams;i++)
		{
			wstring Param;
			if(cfg["Param"+to_str(i)] >> Param)
			{
				// query param 
				wistringstream tis(Param);
				VecRecords rec;
				if(!ReadCVSLineQueryDiv(tis,rec,m_Delims))continue;
				if(!SetupIndexes(rec))continue;

				wistringstream dis(line.c_str());
				if(!ReadCVSLine(dis,rec,m_Delims))continue;

				PInf file;
				VecMask msk;

				int read;

				if(FillInfo(file,rec,msk,&read))
				{
					if(read > max_read)
					{
						best_ind=i;
						max_read=read;
						BestParam=Param;
					}
				}
			}
		}
		if(best_ind >= 0)m_strFormat = BestParam;
		SetupIndexes(BestParam.c_str());
		return best_ind >= 0;
	}

	int SetupIndexes(dcmapDCSTR szParam)
	{
		VecRecords rec;
		wistringstream is(szParam);
		if(!ReadCVSLineQueryDiv(is,rec,m_Delims))return 0;
		return SetupIndexes(rec);
	}
	void RestoreIndexes()
	{
		SetupIndexes(m_strFormat.c_str());

		//if(!SetupIndexes(m_strFormat.c_str()))
		//	SetupIndexes(c_szDefaultFormat);
	}
	bool DCMAPAPI SetTextParam(dcmapDCSTR szParam)
	{
		if(!szParam)m_strFormat=L"";
		else m_strFormat = szParam;

		if(m_strFormat.empty() || !SetupIndexes(m_strFormat.c_str()))
		{
			//SetupIndexes(c_szDefaultFormat);
			return false;
		}
		return true;
	}

	void MakePInf(PInf& pinf)
	{
		pinf.clear();
		pinf.resize(ind.size());
		for(int i=0;i<ind.size();i++)
		{
			pinf[i].is_str = false;
			pinf[i].num = 0;
			pinf[i].str = L"";

			if(ind[i] >= 0)
			{			
				DCMapDataType type = m_pData->ColumnDataType(i);
				pinf[i].is_str = !m_pData->ColumnNumeric(i); 

				if(type == DCMDT_Reference)
				{
					pinf[i].is_str = true;
					if(TableRel[i])
					{
						IDCMapDataStorage* stor = TableRel[i];
						if(m_pData== stor)
						{
							dcmapDBREF ref = *m_pData->GetID();
							if(stor->SelectID(*m_pData->GetReference(i)))
								pinf[i].str = stor->GetWideName();	
							m_pData->SelectID(ref);
						}
						else
						{
							if(stor->SelectID(*m_pData->GetReference(i)))
								pinf[i].str = stor->GetWideName();	
						}
					}
				}
				else if(pinf[i].is_str)
				{
					pinf[i].str = m_pData->GetWideString(i);
				}
				else
				{
					pinf[i].num = m_pData->GetReal(i);
				}
			}
		}
	}

	string m_strTable;

	bool DCMAPAPI SetDataSourceName(const char* szName)
	{
		m_strTable = szName;

		m_pData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(m_strTable.c_str());
		if(!m_pData)return false;

		// Init indexes
		int n = m_pData->NumColumns();

		TableRel.clear();
		TableRel.resize(n,0);

		ind.clear();
		ind.resize(n,-1);
		keyind.clear();
		for(int i=0;i<n;i++)
		{
			//string str = m_pData->ColumnName(i);
			//if(!str.empty())mapInd[str] = i;

			if(m_pData->ColumnParams(i) & DCMAP_COLUMN_INDEX)
				keyind.push_back(i);

			if(m_pData->ColumnDataType(i) == DCMDT_Reference)
			{
				TableRel[i] =
					m_pCore->GetDataSourceTyped<IDCMapDataStorage>(m_pData->ColumnDataLinkage(i));
			}
		}
		return true;
	}


	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore=core;
		return true;
	}
	virtual bool Convert(PInf& pinf,VecRecords &rec,VecMask& mask){return true;}

	bool FillInfo(PInf& pinf,VecRecords &rec,VecMask& mask,int* pnum=0)
	{
		for(int i=0;i<keyind.size();i++) // check for index present
		{
			int rid = ind[keyind[i]];
			if(rid < 0 || rid >= rec.size() )return false;
		}

		pinf.clear();
		pinf.resize(ind.size());
		mask.clear();
		mask.resize(ind.size(),false);

		for(int i=0;i<ind.size();i++)
		{
			pinf[i].is_str = false;

			DCMapDataType type = m_pData->ColumnDataType(i);
			pinf[i].is_str = !m_pData->ColumnNumeric(i); 

			if(type == DCMDT_Reference)
				pinf[i].is_str = true;

			pinf[i].num = 0;
			pinf[i].str = L"";

			if(ind[i] >= 0 && ind[i] < rec.size())
			{
				if(pinf[i].is_str && rec[ind[i]].size())
				{
					pinf[i].str = rec[ind[i]];
					mask[i] = true;
				}
				else
				{
					//pinf[i].str = rec[ind[i]];
					mask[i] = gsys::lex_convert(pinf[i].num,rec[ind[i]]);
				}
			}
		}

		if(!Convert(pinf,rec,mask))return false;
		if(pnum)
		{
			*pnum=0;
			for(int i=0;i<7;i++)if(mask[i])(*pnum)++;
		}
		return true;
	}

	wstring MakeStr(PInf & pinf)
	{
		wstring str;

		for(int i=0;i<m_Delims.size()+1;i++)
		{
			int vr=-1;
			for(int j=0;j<ind.size();j++)
			{
				if(ind[j] == i)vr=j;
			}
			if(vr>=0)
			{
				if(pinf[vr].is_str)
					str += EncodeCSVItem(pinf[vr].str);
				else 
					str += gsys::to_wstr(pinf[vr].num);
			}
			if(i<m_Delims.size())str += m_Delims[i];
		}
		return str;
	}

	bool Merge(PInf const& p1, PInf const& p2,VecMask const& mask,PInf& r)
	{
		int n = p1.size();

		int rupd = 0;
		r.resize(n);

		for(int i=0;i<n;i++)
		{
			if(!mask[i])r[i] = p1[i];
			else
			{
				r[i] = p2[i];
				if(p1[i].num != p2[i].num || p1[i].str != p2[i].str)
				{
					rupd++;
				}
			}
		}
		return rupd>0;
	}

	bool Compare(PInf const& p1,PInf const& p2,VecMask const& mask)
	{
		int n = p1.size();

		int rupd = 0;
		for(int i=0;i<n;i++)
		{
			if(ind[i]>=0 && (p1[i].num != p2[i].num || p1[i].str != p2[i].str))
			{
				rupd++;
			}
		}
		return rupd>0;
	}

	int num_added;
	int num_insert;

	bool DCMAPAPI PreProcessData(dcmapDCSTR szInputData,int iLines,
		IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)
	{
		if(!szInputData || !szInputData[0])return false; 
		wistringstream is(szInputData);

        DCMapScopeBlockUpdates bl(m_pCore);

		m_vecItems.reserve(iLines);

		PInf db;
		PInf file;

		int num_readed=0;
		num_added=0;
		num_insert=0;

		VecRecords rec;
		int errors=0;
		int line_num=-1;
		bool bParamQueried=false;

		pRsvr->SetupProgress(iLines);

		while(!is.eof())
		{
			line_num++;
			int index = m_vecItems.size();
			VecMask msk;

			if(!bParamQueried && m_strFormat.empty())
			{
				wstring line;
				is >> gsys::wistream_line(line) >> wsp_single_nl_scip;
				wistringstream tis(line);

				if(!ReadCVSLineQueryDiv(tis,rec,m_Delims))break;
				if(SetupIndexes(rec))
				{
					bParamQueried=true;
					continue;
				}
				else
				{
					if(QueryIndexes(line))
					{
						bParamQueried=true;
						wistringstream dis(line.c_str());
						if(!ReadCVSLine(dis,rec,m_Delims))break;

						pRsvr->AddMessage((L"Подобран формат:  "+m_strFormat).c_str());
						pRsvr->AddMessage((L"по строке "+to_wstr(line_num)+L": \""+line+L"\"").c_str());
					}
					else
					{
						errors++;
						continue;
					}
				}
			}
			else 
			{
				if(!ReadCVSLine(is,rec,m_Delims))break;
			}

			pRsvr->UpdateProgress(line_num);


			if(rec.size() == 0)continue;

			if(!FillInfo(file,rec,msk))
			{
				errors++;
				bParamQueried=true;
				pRsvr->AddError((L"Строка "+to_wstr(line_num)+L" не распознана").c_str(),line_num);
				continue;
			}
			num_readed++;

			wstring action;
			wstring p1,p2;

			PInf* res = new PInf;

			m_pData->EditIndex();
			for(int i=0;i<keyind.size();i++)
			{
				int rid = ind[keyind[i]];
				if(rid < 0 || rec[rid].empty())errors++;
				else WriteField(keyind[i],file);
			}
			if(m_pData->SelectIndex()) // update record
			{
				action = L"Изменить ";
				MakePInf(db);
				p1 = L"из " L"\""+MakeStr(db)+L"\"";
				if(options & DCMAP_IMPORT_REPLACE )
				{
					if(!Compare(db,file,msk))
					{
						delete res;
						continue;
					}	
					*res = file;
				}
				else 
				{
					if(!Merge(db,file,msk,*res))
					{
						delete res;
						continue;
					}	
				}

				p2 = L" в " L"\""+MakeStr(*res)+L"\"";
			}
			else // Add new record
			{
				action = L"Добавить ";
				*res = file;
				p1 = L"\""+MakeStr(*res)+L"\"";
				num_insert++;
			}
			pRsvr->AddRecord((action+p1+p2).c_str(),index,line_num);
			//pRsvr->AddRecord((action).c_str(),index,line_num);
			num_added++;

			m_vecItems.push_back(res);

		}
		if(num_readed)
		{
			pRsvr->AddMessage((L"Распознано строк: "+to_wstr(num_readed)).c_str());
			pRsvr->AddMessage((L"Ошибок: "+to_wstr(errors)).c_str());

			if(num_added)
				pRsvr->AddMessage((L"Новых данных: "+to_wstr(num_added)).c_str());
			else
				pRsvr->AddMessage(L"Новых данных не обнаружено.");
		}
		else pRsvr->AddMessage(L"Данные не распознаны.");

		pRsvr->UpdateProgress(iLines);

		return true;
	}
	bool DCMAPAPI BeginExecute()
	{
        m_pCore->LockDataUpdate();
		m_pData->BeginUpdate();
		m_pData->ReserveForNew(num_insert);
		return true;
	}

	void WriteField(int i,PInf& inf)
	{
		if(TableRel[i])
		{
			dcmapDBREF ref;
			if(TableRel[i]->RetrieveByWideName(inf[i].str.c_str(),&ref))
			{
				m_pData->SetReference(i,ref);
			}
			else
			{
				TableRel[i]->Insert();
				TableRel[i]->SetWideName(inf[i].str.c_str());
				if(TableRel[i]->Post(&ref))
				{
					m_pData->SetReference(i,ref);
				}
			}
		}
		else if(inf[i].is_str)
			m_pData->SetWideString(i,inf[i].str.c_str());
		else
			m_pData->SetReal(i,inf[i].num);
	}

	bool DCMAPAPI ExecuteRecord(dcmapDCSTR Text,int id)
	{
		PInf& inf = *m_vecItems[id];

		m_pData->EditIndex();
		for(int i=0;i<keyind.size();i++)
		{
			WriteField(keyind[i],inf);
		}
		if(m_pData->SelectIndex()) // update record
		{
			m_pData->Update();
		}
		else // Add new record
		{
			m_pData->Insert();
		}

		for(int i=0;i<ind.size();i++)
		{
			if(ind[i]>=0)
			{
				WriteField(i,inf);
			}
		}
		m_pData->Post();
		return true;
	}

	void DCMAPAPI EndExecute()
	{
        m_pData->EndUpdate();
        m_pCore->UnlockDataUpdate();
	}

	bool DCMAPAPI ImportData(const char* szInputData)
	{
		return false;
	}

	void DCMAPAPI ClearProcessData()
	{
		_foreach(VecItems,m_vecItems,it)
		{
			delete *it;
		}
		m_vecItems.clear();
	}

	void* DCMAPAPI CastTo(char const* idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapImportProcessor);
		return 0;
	}
	dcmapWCSTR DCMAPAPI GetFileFilters(){return L"Файлы CSV|*.csv";}
	dcmapWCSTR DCMAPAPI GetDisplayName(){return L"Импорт CSV";}
	dcmapDWORD DCMAPAPI  GetImportParams()
	{
		return DCMAP_IMPORT_UNIVERSAL|DCMAP_IMPORT_ALLOW_REPLACE|DCMAP_IMPORT_SORT_RECORDS;
	}
	char const* DCMAPAPI GetConfigSectionName()
	{
		static string ConfigSectionParams;
		ConfigSectionParams = "CSVImport_"+m_strTable;
		return ConfigSectionParams.c_str();
	}
};
DCMAP_REGISTER(CCSVTableImport,"PTextImport","UNI_CSVImp");


int DoCCSVTableImport(dcmapCSTR table,dcmapDCSTR param,bool replace,dcmapDCSTR data)
{
	struct holder : public IDCMapProcessRecordReseiver
	{
		std::vector<int> ids;
		virtual bool DCMAPAPI AddRecord(dcmapDCSTR Text,int id=-1,int text_line=-1){ids.push_back(id);return true;}
	} hld;

	CCSVTableImport imp;
	if(!imp.Init(GetDCMapCore(),0))return -1;

	if(!imp.SetDataSourceName(table))return -1;
	imp.SetTextParam(param);
	if(!imp.PreProcessData(data,0,&hld,replace?DCMAP_IMPORT_REPLACE:0))return -1;

	if(hld.ids.empty())return 0;

	imp.BeginExecute();
	_foreach(std::vector<int>,hld.ids,it){imp.ExecuteRecord(0,*it);}
	imp.EndExecute();

	return hld.ids.size();
}

