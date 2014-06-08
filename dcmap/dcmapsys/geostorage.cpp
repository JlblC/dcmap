#include "stdafx.h"
#include "geostorage.h"
#include "PlanetDataStorage.h"


void DCMAPAPI CGeoStorage::Clear()
{
	custom_storage::Clear();

	if(m_NumRecords)
	{
		DCMapPlanetGeoInfo geo = {0,0,0,0,0};
		m_data.fill(geo);
	}
	m_NumRecords=0;
}
CGeoStorage::CGeoStorage(void)
{
	DCMapPlanetGeoInfo geo = {0,0,0,0,0};
	m_data.fill(geo);
	m_NumRecords=0;

	EnumMode=false;
	m_BR.RegisterInt("id",&value_type::id,0);

	m_BR.RegisterInt("x",&value_type::x,DCMDB_PLANET_X);
	m_BR.RegisterInt("y",&value_type::y,DCMDB_PLANET_Y);
	m_BR.RegisterInt("o",&value_type::o,DCMDB_PLANET_O);
	m_BR.RegisterInt("e",&value_type::e,DCMDB_PLANET_E);
	m_BR.RegisterInt("m",&value_type::m,DCMDB_PLANET_M);
	m_BR.RegisterInt("t",&value_type::t,DCMDB_PLANET_T);
	m_BR.RegisterInt("s",&value_type::s,DCMDB_PLANET_S);


	m_BR.SetParam(DCMAP_COLUMN_INDEX,DCMDB_PLANET_X);
	m_BR.SetParam(DCMAP_COLUMN_INDEX,DCMDB_PLANET_Y);

	m_TableVer = 2;
}

void DCMAPAPI CGeoStorage::PostInit()
{
	m_pPlanets = m_pDatabase->GetDataSourceTyped<CPlanetDataStorage>("planets");
	custom_storage::PostInit();
}

int DCMAPAPI CGeoStorage::OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
{

	return custom_storage::OnMessage(Msg,wParam,lParam,pSender);
}

bool DCMAPAPI CGeoStorage::SaveDataToFile(dcmapFCSTR szFileName,int SaveFlags)
{
	boost::scoped_ptr<gsys::gfile> pfile(OpenFileCreateZ(szFileName,true));
	if(!pfile || !pfile->valid())return false;
	gsys::gfile& file = *pfile;

	DataFileHeader head;
	memset(&head,0,sizeof(head));
	head.magic = c_DBMagic;
	head.NumRecords = m_NumRecords;
	head.RecordSize = sizeof(value_type);
	head.modIndex = m_modIndex;
	head.DBVer = m_DBVer;
	head.TableVer = m_TableVer;
	file << head;
	file.twrite(m_data.buffer(),m_data.size());
	return true;
}

bool CGeoStorage::DoLoadData()
{
	boost::scoped_ptr<gfile> pfile(OpenFileReadZ(m_strFilename.c_str()));
	if(!pfile || !pfile->valid())return false;
	gfile& file = *pfile;

	DataFileHeader head;
	file >> head;
	if(head.magic != c_DBMagic)return false;
	if(head.NumRecords >= max_records) return false;

	m_modIndex = head.modIndex;
	m_baseModIndex = head.modIndex;

	if(head.TableVer > m_TableVer) return false; // incompatible
	if(head.DBVer > m_DBVer) return false; // incompatible

	m_NumRecords = head.NumRecords;
	if(head.TableVer < 2) // Load data from old small world
	{
		typedef gsys::static_array2d<DCMapPlanetGeoInfo,1000,1000> OldData;
		boost::scoped_ptr<OldData> p_data(new OldData);
		file.tread(p_data->buffer(),p_data->size());
		DCMapPlanetGeoInfo geo = {0,0,0,0,0};
		m_data.fill(geo);
		for(int y=0;y<1000;y++)
		{
			for(int x=0;x<1000;x++)
			{
				m_data(x,y) = (*p_data)(x,y);
			}
		}
	}
	else
	{
		file.tread(m_data.buffer(),m_data.size());
	}
	m_bLoaded = true;
	return true;
}

const dcmapDBREF* DCMAPAPI CGeoStorage::GetID()
{
	static dcmapDBREF ref;
	if(!m_pCurrentRecord) return &ref;
	return &m_Record.id;
}

bool DCMAPAPI CGeoStorage::SelectCoord(int x,int y)
{
	if(!DataLoad())return false;
	if(!TestCoords(x,y))return false;
	m_pCurrentRecord=0;
	EnumMode=false;
	m_Record.geo = m_data(x-1,y-1);
	if(!m_Record.s)return false;
	m_Record.x = x;
	m_Record.y = y;
	m_Record.id.id = dcmapMAKELONG(x,y);
	m_Record.id.age = m_Record.id.id;
	m_pCurrentRecord = &m_Record;
	return true;
}
bool DCMAPAPI CGeoStorage::SelectIndex()
{
	if(!DataLoad())return false;

	m_bEditIndex=false;
	return SelectCoord(m_temp.x,m_temp.y);
}

bool DCMAPAPI CGeoStorage::SelectID(const dcmapDBREF &ref)
{
	if(!DataLoad())return false;

	if(ref.age != ref.id)return false;
	return SelectCoord(dcmapLOWORD(ref.id),dcmapHIWORD(ref.id));
}

bool CGeoStorage::LocatePlanet()
{
	DCMapPlanetGeoInfo* row = m_data.row(m_Record.y-1);
	while(1)
	{
		if(m_Record.x > m_iEndX)
		{
			m_Record.x=1;
			m_Record.y++;
			if(m_Record.y > m_iEndY)return false;
			row = m_data.row(m_Record.y-1);
		}
		if(row[m_Record.x-1].s)
		{
			m_Record.geo = row[m_Record.x-1];
			m_Record.id.id = dcmapMAKELONG(m_Record.x,m_Record.y);
			m_Record.id.age = m_Record.id.id;
			m_pCurrentRecord = &m_Record;
			return true;
		}
		m_Record.x++;
	}
}

bool DCMAPAPI CGeoStorage::SelectAll()
{
	if(!DataLoad())return false;

	if(!m_NumRecords)return false;
	m_Record.x=1;
	m_Record.y=1;

	m_iEndX=g_DCMapNumPlanets;
	m_iEndY=g_DCMapNumPlanets;

	if(!LocatePlanet())return false;
	EnumMode = true;
	return true;
}

bool DCMAPAPI CGeoStorage::Next()
{
	if(!EnumMode)return false;
	m_Record.x++;
	if(!LocatePlanet())
	{
		EnumMode = false;
		m_pCurrentRecord=0;
		return false;
	}
	return true;
}

bool DCMAPAPI CGeoStorage::Delete()
{
	DCMapPlanetGeoInfo geo = {0,0,0,0,0};
	m_data(m_Record.x-1,m_Record.y-1) = geo;
	m_pCurrentRecord=0;
	m_NumRecords--;
	DataChanged();		
	return true;
}

bool DCMAPAPI CGeoStorage::Post(dcmapDBREF * ref)
{
	if(!TestCoords(m_temp.x,m_temp.y))return false;
	if(m_temp.s==0)return false;

	m_data(m_temp.x-1,m_temp.y-1) = m_temp.geo;
	if(m_bInsertMode)m_NumRecords++;
	m_bInsertMode=false;
	m_bUpdateMode=false;
	m_Record = m_temp;
	m_Record.id.id = dcmapMAKELONG(m_Record.x,m_Record.y);
	m_Record.id.age = m_Record.id.id;
	m_pCurrentRecord=&m_Record;
	ref = &m_Record.id;
	DataChanged();		
	return true;
}

void CGeoStorage::OnChange()
{
	if(m_pCurrentRecord) // update geo information
	{
		DCMapPlanetInfo* pInf = m_pPlanets->GetDataByCoords(m_pCurrentRecord->x,m_pCurrentRecord->y);
		if(pInf)
		{
			//if(!(pInf->geo == m_pCurrentRecord->geo))
			if(pInf->geo.s == 0)
			{
				pInf->geo = m_pCurrentRecord->geo;
				m_pPlanets->m_modIndex++;
			}
		}
	}
}

bool DCMAPAPI  CGeoStorage::SelectLine(int LineNum,int From,int To)
{
	if(!DataLoad())return false;

	m_Record.y = LineNum;
	m_Record.x = From;

	m_iEndX=To-1;
	m_iEndY=LineNum;

	if(!LocatePlanet())return false;
	EnumMode = true;
	return true;
}

bool DCMAPAPI CGeoStorage::UpdateGeo( const DCMapPlanetParamInfo* data )
{
	if(!data)return false;
	if(!TestCoords(data->x,data->y))return false;
	if(!DataLoad())return false;

	m_data(data->x-1,data->y-1) = data->geo;
	return true;
}
DCMAP_REGISTER_STORAGE(CGeoStorage,"geo");
