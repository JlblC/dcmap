#pragma once
#include "../dcmapsys.h"
#include <boost/regex.hpp>

class CRegExpRecordImport
{
	IDCMapDataStorage* m_pData;
	IDCMapCore* m_pCore;
	typedef vector<IDCMapDataStorage*> VecRelations;
	VecRelations TableRel;

	typedef vector<int> VecIndexes;
	VecIndexes keyind;
	VecIndexes ind;
	typedef map<string,wstring> MapVars;
	MapVars m_vars;
	typedef pair<int,wstring> PInf;
	typedef vector<PInf> VecInfo;
	VecInfo m_vecInfo;

	dcmapDCSTR m_strSeqBegin;
	dcmapDCSTR m_strSeqEnd;

	bool Parse(wistream& is,dcmapDCSTR strBegin,dcmapDCSTR strEnd);
	int ParseValue(wistream& is,boost::wcmatch &rvars,wstring& result,int& type);
	bool ParseRegExp(wistream& is,dcmapDCSTR strBegin,dcmapDCSTR strEnd,boost::wcmatch &rvars);

public:
	~CRegExpRecordImport(void);
	CRegExpRecordImport(void);

	void DCMAPAPI SetVariableW(dcmapCSTR strName,dcmapDCSTR strValue);
	void DCMAPAPI SetVariable(dcmapCSTR strName,dcmapCSTR strValue);

	bool DCMAPAPI Init(IDCMapCore* pCore,IDCMapDataStorage* pData);
	bool DCMAPAPI Parse(dcmapDCSTR strData,dcmapDCSTR strRegExp);
	bool DCMAPAPI Import(dcmapDBREF const* CurrentRecord=0);
	bool DCMAPAPI ImportSeq(dcmapDCSTR strData,dcmapDCSTR strRegExp);
};
