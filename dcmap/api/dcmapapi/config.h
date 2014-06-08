
struct IDCMapConfig
{
	virtual void DCMAPAPI RemoveSection(dcmapCSTR szSection)=0;
	virtual void DCMAPAPI RemoveEntry(dcmapCSTR szSection)=0;

	virtual void DCMAPAPI SelectSection(dcmapCSTR szSection)=0;

	virtual dcmapCSTR DCMAPAPI ReadString(dcmapCSTR szSection)=0;
	virtual void DCMAPAPI WriteString(dcmapCSTR szSection,char const* str)=0;

	virtual dcmapWCSTR DCMAPAPI ReadStringW(dcmapCSTR szSection)=0;
	virtual void DCMAPAPI WriteStringW(dcmapCSTR szSection,dcmapWCSTR str)=0;

	virtual bool DCMAPAPI ReadInteger(dcmapCSTR szName,int* data)=0;
	virtual bool DCMAPAPI ReadUnsigned(dcmapCSTR szName,dcmapDWORD* data)=0;
	virtual void DCMAPAPI WriteInteger(dcmapCSTR szName,int data)=0;

	virtual bool DCMAPAPI ReadFloat(dcmapCSTR szName,double* data)=0;
	virtual void DCMAPAPI WriteFloat(dcmapCSTR szName,double data)=0;

	virtual bool DCMAPAPI ReadInt64(dcmapCSTR szName,dcmapINT64* data)=0;
	virtual void DCMAPAPI WriteInt64(dcmapCSTR szName,dcmapINT64 data)=0;

	virtual bool DCMAPAPI ReadUnsignedInt64(dcmapCSTR szName,dcmapUINT64* data)=0;
	virtual void DCMAPAPI WriteUnsignedInt64(dcmapCSTR szName,dcmapUINT64 data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapConfig);


//////////////////////////////////////////////////////////////////////////
class DCMapConfigWrapper
{
	IDCMapConfig* m_pConfig;
public:
	class Record
	{
		IDCMapConfig* m_pConfig;
		const char* rec;
	public:
		Record(IDCMapConfig* pConfig,const char* sz):m_pConfig(pConfig),rec(sz){};
		IDCMapConfig* config(){return m_pConfig;}
		const char* record(){return rec;}

		void operator << (int data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (int &data){return m_pConfig->ReadInteger(rec,&data);}

		void operator << (long data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (long &data){return m_pConfig->ReadInteger(rec,(int*)&data);}

		void operator << (unsigned int data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (unsigned int &data){return m_pConfig->ReadInteger(rec,(int*)&data);}

		void operator << (unsigned long data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (unsigned long &data){return m_pConfig->ReadInteger(rec,(int*)&data);}

		void operator << (bool data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (bool &data){int v;if(m_pConfig->ReadInteger(rec,&v)){data = (v!=0);return true;}return false;}

		void operator << (short data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (short &data){int v;if(m_pConfig->ReadInteger(rec,&v)){data = v;return true;}return false;}

		void operator << (unsigned short data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (unsigned short &data){int v;if(m_pConfig->ReadInteger(rec,&v)){data = v;return true;}return false;}

		void operator << (char data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (char &data){int v;if(m_pConfig->ReadInteger(rec,&v)){data = v;return true;}return false;}

		void operator << (unsigned char data){m_pConfig->WriteInteger(rec,data);}
		bool operator >> (unsigned char &data){int v;if(m_pConfig->ReadInteger(rec,&v)){data = v;return true;}return false;}

		void operator << (float data){m_pConfig->WriteFloat(rec,data);}
		bool operator >> (float &data){double v;if(m_pConfig->ReadFloat(rec,&v)){data = (float)v;return true;}return false;}

		void operator << (double data){m_pConfig->WriteFloat(rec,data);}
		bool operator >> (double &data){double v;if(m_pConfig->ReadFloat(rec,&v)){data = v;return true;}return false;}

		void operator << (dcmapINT64 data){m_pConfig->WriteInt64(rec,data);}
		bool operator >> (dcmapINT64 &data){dcmapINT64 v;if(m_pConfig->ReadInt64(rec,&v)){data = v;return true;}return false;}

		void operator << (dcmapUINT64 data){m_pConfig->WriteUnsignedInt64(rec,data);}
		bool operator >> (dcmapUINT64 &data){dcmapUINT64 v;if(m_pConfig->ReadUnsignedInt64(rec,&v)){data = v;return true;}return false;}

#ifdef DCMAP_VCL
		void operator << (Extended data){m_pConfig->WriteFloat(rec,data);}
		bool operator >> (Extended &data){double v;if(m_pConfig->ReadFloat(rec,&v)){data = v;return true;}return false;}
#endif

#ifdef DCMAP_STD_STRING
		// vcl extension to ConfigWrapper
		void operator << (std::string const&  data)
		{
			config()->WriteString(record(),data.c_str());
		}
		bool operator >> (std::string &data)
		{
			char const* sz = config()->ReadString(record());
			if(sz)data = sz;
			return sz !=0;
		}
		void operator << (dcmap::wstring const&  data)
		{
			config()->WriteStringW(record(),data.c_str());
		}
		bool operator >> (dcmap::wstring &data)
		{
			dcmapWCSTR sz = config()->ReadStringW(record());
			if(sz)data = sz;
			return sz !=0;
		}
#endif

#ifdef DCMAP_VCL
		// vcl extension to ConfigWrapper
		void operator << (AnsiString const&  data)
		{
			config()->WriteString(record(),data.c_str());
		}
		bool operator >> (AnsiString &data)
		{
			char const* sz = config()->ReadString(record());
			if(sz)data = sz;
			return sz !=0;
		}
		void operator << (WideString const&  data)
		{
			config()->WriteStringW(record(),data.c_bstr());
		}
		bool operator >> (WideString &data)
		{
			dcmapDCSTR sz = config()->ReadStringW(record());
			if(sz)data = sz;
			return sz !=0;
		}

		void operator << (UnicodeString const&  data)
		{
			config()->WriteStringW(record(),WideString(data).c_bstr());
		}
		bool operator >> (UnicodeString &data)
		{
			dcmapDCSTR sz = config()->ReadStringW(record());
			if(sz)data = sz;
			return sz !=0;
		}

		bool operator >> (TStrings* &data)
		{
			dcmapCSTR sz = config()->ReadString(record());
			if(sz)data->Text = sz;
			return sz !=0;
		}
		void operator << (TStrings* &data)
		{
			config()->WriteStringW(record(),data->Text.w_str());
		}
#endif
	};
	DCMapConfigWrapper(IDCMapConfig* pConfig):m_pConfig(pConfig){};
	explicit DCMapConfigWrapper(IDCMapCore* pCore):m_pConfig(pCore->GetConfig()){};
	explicit DCMapConfigWrapper(IDCMapWorkspace* pWorkspace):m_pConfig(pWorkspace->GetConfig()){};
	Record operator[](const char* id){return Record(m_pConfig,id);}

#ifdef INC_VCL
	Record operator[](AnsiString const& id){return Record(m_pConfig,id.c_str());}
	void section(AnsiString const& id){m_pConfig->SelectSection(id.c_str());}
#endif 

#ifdef DCMAP_STD_STRING
	Record operator[](std::string const& id){return Record(m_pConfig,id.c_str());}
	void section(std::string const& id){m_pConfig->SelectSection(id.c_str());}
#endif 

	IDCMapConfig* operator->(){return m_pConfig;}

	void section(const char* id)
	{
		m_pConfig->SelectSection(id);
	}
};
