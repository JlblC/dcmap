#pragma once

// dcmapsys.cpp : Defines the entry point for the DLL application.
//

wstring EncodeCSVItem(wstring const& str,wchar_t delim=0);

bool ReadCVSLineQueryDiv(std::wistream& is,std::vector<std::wstring>& items,
								std::vector<wchar_t>& delims);
bool ReadCVSLine(std::wistream& is,std::vector<std::wstring>& items,std::vector<wchar_t> const& delims);

std::wistream& wsp_single_nl_scip(std::wistream& is);