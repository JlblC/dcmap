#pragma once

class CGeckoManager: public IDCMapModule
{
protected:
	wstring m_strGeckoLocation;
public: // base interface
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	CGeckoManager();
	virtual ~CGeckoManager();
	bool  DCMAPAPI OnCreate(DCMapObjectCreationParams const* params);
	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

public:
};
DCMAP_SPEC_INTERFACE(CGeckoManager);
