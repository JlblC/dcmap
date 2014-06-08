#pragma once

#include "parsestream.h"
#include "utils.h"

enum EVarType{VT_NIL,VT_NUM,VT_STR,VT_BIN,VT_VEC,VT_HASH,VT_SUB,VT_MODULE,VT_DBREF,VT_OBJECT,VT_PDATA,VT_SERVICE};
class CVarValue;
class CBasicBlockParser;
class CObjectParser;


inline wstring ftow(double v)
{
	static dcmapCHAR buf[64];
	sprintf(buf,"%.16g",v);
	return ToWStr(buf);
}

typedef CObjectParser* (*ParseObjectCreatorCallback)();

struct PrivateDataDef
{
	void* pData;
};

struct SubDef
{
	int   pos;
    ParseObjectCreatorCallback creator;
	wiparsestream* stream; // stream context for executing sub
	CObjectParser* context;
	void Execute(CVarValue const& param,CVarValue & res);
    void Acquire(CObjectParser* mod);
    void Disacquire();
};

struct ModuleDef : public SubDef
{
   /* int pos;
    wiparsestream* stream; // stream context for executing module
    CModuleParser* context;*/
    void Execute(CVarValue const& param,CVarValue & res);
};

struct ServiceDef 
{
	IDCMapScriptable* pmod;
	int type;
	CVarValue subobj(CVarValue const& v,wstring &str);
	CVarValue subobj( CVarValue const& v);
	CVarValue retrieve_member( CVarValue const& v,wstring &str);
	bool Assign(wstring const& str,CVarValue const& v);
	void Execute(wstring const& str,CVarValue const& param,CVarValue & res);
	bool have_subobj( CVarValue const& v,wstring &str );
};

struct ObjectRef
{
    CObjectParser* pModule;
    void clear(){pModule=0;}
    CVarValue get();
    CVarValue get(CVarValue const& v);
    bool set(CVarValue const& v);
    bool retrieve(CVarValue const& v,CVarValue ** result);
    void free();
    void clone(const ObjectRef& obj);
    IDCMapModule* object_module();
    void add_ref();
};

struct DBRef
{
    dcmapQWORD		   ref;
	IDCMapDataStorage* storage;
};

struct BinDataRef;

typedef void (*BinDataRefOperatorCallback)(BinDataRef const* src,BinDataRef* dst);

struct BinDataRef
{
    int param;
    BinDataRefOperatorCallback oper;
    dcmapBYTE* pbegin;
    dcmapBYTE* pend;
    void store(dcmapBYTE* _pbegin, dcmapBYTE* _pend)
    {
        unlink();
        pbegin = new dcmapBYTE[_pend-_pbegin];
        pend = pbegin+(_pend-_pbegin);
        oper = 0;
    }
    void store(BinDataRef const& v)
    {
        unlink();
        if(v.oper)
        {
            v.oper(&v,this);
        }
        else
        {
            pbegin = new dcmapBYTE[v.pend-v.pbegin];
            pend = pbegin+(v.pend-v.pbegin);
            oper = 0;
        }
    }   
    void unlink()
    {
        if(oper) oper(0,this);
        else if(pbegin)delete[] pbegin;
        pbegin = pend = NULL;
        oper = NULL;
    }
};

typedef std::map<CVarValue,CVarValue> CVarValueMap;

class CVarValue
{
    static const CVarValue nil_value;
protected:
	struct CVarData
	{
		EVarType type;

		union
		{
			mutable double num;
            mutable SubDef sub;
			mutable ModuleDef moddef;
            mutable ObjectRef  object;
			mutable DBRef  dbref;
            mutable BinDataRef  binref;
			mutable PrivateDataDef  pdata;
			mutable ServiceDef  service;
		};
		mutable std::wstring str;   
		std::vector<CVarValue> array;
		CVarValueMap hash;
		int Refs;

		CVarData():Refs(1),type(VT_NIL),num(invalid_num()){}
		explicit CVarData(double v):Refs(1),type(VT_NUM),num(v){};
		explicit CVarData(wstring const& v):Refs(1),type(VT_STR),num(invalid_num()),str(v){};
        explicit CVarData(string const& v):Refs(1),type(VT_STR),num(invalid_num()),str(StrC2W(v)){};
		explicit CVarData(dcmapWCSTR v):Refs(1),type(VT_STR),num(invalid_num()),str(v?v:L""){};
        explicit CVarData(dcmapCSTR v):Refs(1),type(VT_STR),num(invalid_num()),str(ToWide(v)){};

        explicit CVarData(dcmapDBREF const& ref,IDCMapDataStorage* storage=0):
         			Refs(1),type(VT_DBREF),num(invalid_num())
			{
			dbref.ref = ref.data;
			dbref.storage=storage;
			};

		explicit CVarData(IDCMapScriptable* pmod):Refs(1),type(VT_SERVICE){service.pmod = pmod;service.type = 0;};
		explicit CVarData(IDCMapScriptable* pmod,int type,wstring const& m):Refs(1),type(VT_SERVICE),str(m){service.pmod = pmod;service.type=type;};

		explicit CVarData(IDCMapObject* pObject):Refs(1),type(VT_SERVICE){service.pmod = (IDCMapScriptable*)pObject;service.type = 0;};

        CVarData(CVarData const& v):Refs(1),type(v.type),str(v.str),array(v.array),hash(v.hash)
        {
            if(type == VT_OBJECT)
            {
                object.clone(v.object);
            }
            else if(type == VT_SUB)
            {
                sub = v.sub;
                sub.Acquire(v.sub.context);
            }
            else if(type == VT_MODULE)
            {
                moddef = v.moddef;
                moddef.Acquire(v.moddef.context);
            }
            else if(type == VT_BIN)
            {
                binref.store(v.binref);
            }            
			else if(type == VT_SERVICE)
			{
				service = v.service;
			}  
			else sub = v.sub;
        }

		void LVAssign(CVarValue const& v)
		{
			if(type == VT_SERVICE)
			{
				service.Assign(str,v);
			}  
		}

        ~CVarData()
        {
            free();
        }

        void free()
        {
            if(type == VT_OBJECT)object.free(); 
            else if(type == VT_SUB)sub.Disacquire(); 
            else if(type == VT_MODULE)moddef.Disacquire(); 
            else if(type == VT_BIN)binref.unlink(); 
        }

		bool to_bool()const
		{
            if(type == VT_OBJECT)return object.get().to_bool();
			if(type == VT_VEC)return get(0).to_bool();
			if(type == VT_NUM)return num != 0;
			if(type == VT_STR)return !str.empty();
            if(type == VT_OBJECT)return object.get().to_bool();
			return false;
		}
		void append(CVarValue const& val)
		{
			if(type == VT_HASH)
			{
				if(val.type() == VT_HASH)
				{
					_const_foreach(CVarValueMap,val.get_hash(),it)
					{
						hash[it->first] = it->second;
					}
				}
				else if(val.type() == VT_VEC)
				{
					int sz= val.size();
					for(int i=0;i<sz;i+=2)
					{
						hash[val[i]] = val[i+1];
					}
				}
			}
			else 
			{
				if(type != VT_VEC)
				{
					array.clear();
					array.push_back(CVarValue(this)); // move raw data to other VarValue
                    free();
                    type = VT_VEC;
				}
				for(int i=0;i<val.size();i++)array.push_back(val[i]);
			}
		}
        void append_value(CVarValue const& val)
        {
            if(type != VT_VEC)
            {
                array.clear();
                array.push_back(CVarValue(*this)); // move raw data to other VarValue
                free();
                type = VT_VEC;
            }
            array.push_back(val);
        }
		bool is_nil()const{return type == VT_NIL;}
		CVarValue get(int i)const
		{
			if(type == VT_VEC)
			{
				unsigned ind=i;
				if(i<0)ind = array.size()-i;

				if(ind >= array.size())return nil_value;
				else return array[ind];
			}
			else if(i==0)
			{
				return CVarValue((CVarData*)this); 
			}
			return nil_value;
		}

		bool have_subobj_writable(wstring const& name)const
		{
			if(type == VT_HASH)
			{
				CVarValueMap::const_iterator it = hash.find(CVarValue(name));
				return it != hash.end();
			}
			else if(type == VT_SERVICE)
			{
				return service.have_subobj(CVarValue(name),str);
			}
			return false;
		}
		bool have_subobj(wstring const& name)const
		{
			if(type == VT_HASH)
			{
				CVarValueMap::const_iterator it = hash.find(CVarValue(name));
				return it != hash.end();
			}
			else if(type == VT_SERVICE)
			{
				return service.have_subobj(CVarValue(name),str);
			}
			return false;
		}

        CVarValue subobj(CVarValue const& v)const
        {
			if(type == VT_OBJECT) return object.get(v);
			else if(type == VT_SERVICE) return service.subobj(v,str);
            else if(type == VT_HASH) return get(v);
            return CVarValue();
        }

		CVarValue get(CVarValue const& v)const
		{
			if(type == VT_HASH)
			{
				int sz = v.size();
				if(sz == 1)
				{
					CVarValueMap::const_iterator it = hash.find(v);
					if(it == hash.end())return CVarValue();
					return it->second;
				}
                else
				{
					CVarValue r;
					r.set_vector();
					for(int i=0;i<sz;i++)
					{
						CVarValueMap::const_iterator it = hash.find(v[i]);
						if(it == hash.end())r.append(CVarValue());
						else r.append(it->second);
					}
					return r;
				}
			}
			else
			{
				int sz = v.size();
				if(sz==1)
				{
					return get(v.to_real());
				}
				else 
				{
					CVarValue r;
					r.set_vector();
					for(int i=0;i<sz;i++)
					{
						r.append(get(v[i].to_real()));
					}
					return r;
				}
			}
		}

		CVarValue retrieve_var(CVarValue const& v)const
		{
			if(type == VT_OBJECT) return object.get(v);
			else if(type == VT_SERVICE) return service.retrieve_member(v,str);
			else if(type == VT_HASH) return get(v);
			return CVarValue();
		}

		bool retrieve(CVarValue const& v,CVarValue ** result)
		{
            if(type == VT_OBJECT)
                return object.retrieve(v,result);

			if(type == VT_HASH)
			{
				*result = &hash[v];
				return true;
			}			
			unsigned i = v.to_real();
			if(type == VT_VEC)
			{
				if(i > array.size())array.resize(i+1);
				*result = &array[i];
				return true;
			}
			if(i==0)return false;
			array.push_back(CVarValue(*this));
			array.resize(i+1);
			type = VT_VEC;
			*result = &array[i];
			return true;
		}

		int size()const
		{
			if(type == VT_VEC)return array.size();
            else if(type == VT_HASH)return hash.size();
			else return 1;
		}
		const std::wstring& as_string()const
		{
            if(type == VT_OBJECT)return object.get().as_string();
			else if(type == VT_VEC)str = get(0).to_string();
			else if(type != VT_STR && str.empty())
			{
				//if(type == VT_NUM)str = gsys::lex_cast<wstring>(num);
				if(type == VT_NUM)str = ftow(num);
			}
			return str;
		}
        std::wstring& as_string()
        {
            if(type != VT_STR && str.empty())
            {
                if(type == VT_NUM)
				{
					/*
					static dcmapWCHAR buf[_CVTBUFSIZE];
										str = swprintf(buf,L"%#.25G",num); 
										str = buf;*/
					//str = gsys::lex_cast<std::wstring>(num);

					str = ftow(num);
				}
            }
            return str;
        }		
        
        double as_real()const
		{
            if(type == VT_OBJECT)return object.get().as_real();
			else if(type == VT_VEC)return get(0).to_real();
			else if(type == VT_STR && num == invalid_num())
			{
				num = _wtof(str.c_str());
                return num;
			}
			else if(type == VT_NUM)return num;
			return 0;
		}
	};	

	CVarData* data;
	static double invalid_num(){return std::numeric_limits<double>::infinity();}

	void take_ownership()
	{
		if(data)
		{
			if(data->Refs <= 1)return;
			data->Refs--;
            data = new CVarData(*data);
		}
        else
        {
            data = new CVarData();
        }
	}
	void clear_data()
	{
		if(data)
		{
			if(data->Refs <= 1)
			{
                data->free();
				data->num = invalid_num();
				data->str.clear();
				data->array.clear();
				data->hash.clear();
                data->type = VT_NIL;
				return;
			}
			else data->Refs--;
		}
		data = new CVarData();
	}
	explicit CVarValue(CVarData const* v)
	{
		if(v->is_nil())data=0;
		else data = new CVarData(*v);
	}
	explicit CVarValue(CVarData * v)
	{
        data = v;
        if(data)data->Refs++;
	}
public:
	CVarValue():data(0){}
	explicit CVarValue(double v):data(new CVarData(v)){};
	explicit CVarValue(std::wstring const& v):data(new CVarData(v)){};
	explicit CVarValue(dcmapWCSTR v):data(new CVarData(v)){};
    explicit CVarValue(std::string const& v):data(new CVarData(v)){};
    explicit CVarValue(dcmapCSTR v):data(new CVarData(v)){};
    explicit CVarValue(bool v):data(new CVarData(v)){};
    explicit CVarValue(int v):data(new CVarData(v)){};
    explicit CVarValue(unsigned int v):data(new CVarData(v)){};
    explicit CVarValue(long v):data(new CVarData(v)){};
    explicit CVarValue(unsigned long v):data(new CVarData(v)){};
    explicit CVarValue(dcmapDBREF const& ref,IDCMapDataStorage* storage=0):data(new CVarData(ref,storage)){};
	explicit CVarValue(IDCMapScriptable* pmod){if(pmod)data= new CVarData(pmod);else data = 0;};
	explicit CVarValue(IDCMapScriptable* pmod,int type,wstring const& kw){if(pmod)data= new CVarData(pmod,type,kw);else data = 0;};
	explicit CVarValue(IDCMapObject* pmod){if(pmod)data= new CVarData(pmod);else data = 0;};

    CVarValue& operator = (double v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
	CVarValue& operator = (std::wstring const& v){set_string(v);return *this;}
	CVarValue& operator = (dcmapWCSTR v){set_string(v);return *this;}
    CVarValue& operator = (bool v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
    CVarValue& operator = (int v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
    CVarValue& operator = (unsigned int v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
    CVarValue& operator = (long v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
    CVarValue& operator = (unsigned long v){clear_data();data->type = VT_NUM;data->num = v;return *this;}
	CVarValue& operator = (IDCMapScriptable* pmod)
	{
		if(!pmod)set_nil();
		else
		{
			clear_data();
			data->type = VT_SERVICE;
			data->service.pmod = pmod;
			data->service.type = 0;
		}
		return *this;
	}

	CVarValue(CVarValue const& v)
	{
		data = v.data;
		if(data)data->Refs++;
	}
	CVarValue(CVarData const& v){data = new CVarData(v);}

	CVarValue& operator = (CVarValue const& v)
	{
		set_nil();
		data = v.data;
		if(data)data->Refs++;
		return *this;
	}

	~CVarValue()
	{
		set_nil();
	}

	void LVAssign(CVarValue const& v)
	{
		if(!data || data->type != VT_SERVICE)
		{
			set_nil();
			data = v.data;
			if(data)data->Refs++;
		}
		else
		{
			data->LVAssign(v);
		}
	}
// -----------------------------------------------------------------------------------------------------
	friend bool operator < (CVarValue const& v1,CVarValue const& v2) // operator for map accesing
	{
        if(v1.data == v2.data)return false;
		if(v1.is_nil()) return !v2.is_nil();
		if(v2.is_nil()) return false;
		if(v1.numeric() && v2.numeric())return v1.to_real() < v2.to_real();
		return v1.as_string() < v2.as_string();
	}

	friend bool operator > (CVarValue const& v1,CVarValue const& v2) // operator for map accesing
	{
		if(v1.data == v2.data)return false;
		if(v2.is_nil()) return !v1.is_nil();
		if(v1.is_nil()) return false;
		if(v1.numeric() && v2.numeric())return v1.to_real() > v2.to_real();
		return v1.as_string() > v2.as_string();
	}

	friend bool operator == (CVarValue const& v1,CVarValue const& v2) // operator for map accesing
	{
		if(v1.data == v2.data)return true;
		if(v1.is_nil()) return v2.is_nil();
		if(v2.is_nil()) return false;
		if(v1.numeric() && v2.numeric())return v1.to_real() == v2.to_real();
		return v1.as_string() == v2.as_string();
	}
	// -----------------------------------------------------------------------------------------------------


	void set_nil()
	{
		if(data)
		{
			data->Refs--;
			if(data->Refs <= 0)delete data;
			data = 0;
		}
	}

	bool can_have_subs()
	{
		if(data && (data->type == VT_VEC 
			|| data->type == VT_HASH 
			|| data->type == VT_SERVICE
			|| data->type == VT_OBJECT ))
			return true;

		return false;
	}

    bool is_scalar()
    {
        if(data && (data->type == VT_VEC || data->type == VT_HASH))
            return false;
        return true;
    }

	bool is_dbref()
	{
		if(data && (data->type == VT_DBREF))
			return true;
		return false;
	}

    void swap(CVarValue& val)
    {
        gsys::exchange(data,val.data);
    }
    void eat(CVarValue& val)
    {
        if(data)
        {
            data->Refs--;
            if(data->Refs <= 0)delete data;
        }
        data = val.data;
        val.data = 0;
    }

	void append(CVarValue const& val)
	{
        if(is_scalar())
        {
            CVarValue sv;
            swap(sv);
            data = new CVarData();
            data->type = VT_VEC;
            data->array.push_back(sv);
            data->append(val);
        }
        else
        {
		    take_ownership();
		    data->append(val);
        }
	}

    void append_value(CVarValue const& val)
    {
        if(is_scalar())
        {
            CVarValue sv;
            swap(sv);
            data = new CVarData();
            data->type = VT_VEC;
            data->array.push_back(sv);
            data->append_value(val);
        }
        else
        {
            take_ownership();
            data->append_value(val);
        }
    }

	std::wstring& set_string()
	{
		clear_data();
		data->type = VT_STR;
		return data->str;
	}
    std::wstring& set_string(dcmapWCSTR v)
    {
        clear_data();
        data->type = VT_STR;
        if(v)data->str=v;
        return data->str;
    }
	std::wstring& set_string(std::wstring const& v)
	{
		clear_data();
		data->type = VT_STR;
		data->str = v;
		return data->str;
	}

	IDCMapScriptable * get_scriptable()
	{
		if(!data)return 0;
		if(data->type != VT_SERVICE)return 0;
		return data->service.pmod;
	}

	IDCMapObject * get_sysobject()
	{
		if(!data)return 0;
		if(data->type != VT_SERVICE)return 0;
		return data->service.pmod;
	}

	bool to_bool()const
	{
		if(!data)return false;
		return data->to_bool();
	}

	std::wstring to_string()const
	{
		if(!data)return L"";
		return data->as_string();
	}
	double to_real()const
	{
		if(!data)return 0;;
		return data->as_real();
	}
	int to_int()const
	{
		if(!data)return 0;
		return data->as_real();
	}
	double as_real()const
	{
		if(!data)return 0;;
		return data->as_real();
	}

    dcmapDBREF const& as_dbref()const
    {
        static dcmapDBREF sref;
        if(type() != VT_DBREF)return sref;
        return *((dcmapDBREF const*)&data->dbref.ref);
    }

    IDCMapDataStorage* dbstorage()const
    {
        static dcmapDBREF sref;
        if(type() != VT_DBREF)return 0;
        return data->dbref.storage;
    }

	const std::wstring& as_string()const
	{
		static wstring defstr;
		if(!data)return defstr;
		return data->as_string();
	}

    std::wstring& edit_string()
    {
        if(!data) return set_string();
        else if(data->type != VT_STR)
        {
            wstring dta = data->as_string();
            return set_string(dta);
        }
        else 
        {
            take_ownership();
            return data->str;
        }
    }

	bool is_nil()const{return !data;}
	bool numeric()const
	{
		if(!data)return 1;
		return data->type == VT_NUM;
	}

	EVarType type()const
	{
		if(!data)return VT_NIL;
		return data->type;
	}

	bool have_subobj(wstring const& name)const
	{
		if(!data)return false;
		return data->have_subobj(name);
	}
	bool have_subobj_writable(wstring const& name)const
	{
		if(!data)return false;
		return data->have_subobj_writable(name);
	}
	CVarValue operator[](int i)const
	{
        if(type() != VT_VEC)return i?nil_value:*this;
		return data->get(i);
	}

    CVarValue get(int i)const
    {
        if(!data)return *this;
        return data->get(i);
    }

    CVarValue get(dcmapWCSTR i)const
    {
        if(!data)return *this;
        return data->get(CVarValue(i));
    }

	CVarValue operator[](dcmapWCSTR i)const
	{
		if(!data)return *this;
		return data->get(CVarValue(i));
	}

	CVarValue operator[](CVarValue const& v)const
	{
		if(!data)return *this;
		return data->get(v);
	}

    CVarValue subobj(CVarValue const& v)const
    {
        if(!data)return CVarValue();
        return data->subobj(v);
    }

	CVarValue retrieve_var(CVarValue const& v)
	{
		if(!data)return CVarValue();
		return data->retrieve_var(v);
	}

	bool retrieve(CVarValue const& v,CVarValue ** result)
	{
		if(!data)
		{
			if(unsigned(v.to_real())==0)*result = this;
			else
			{
				data = new CVarData();
				if(!data->retrieve(v,result))
				{
					*result = this;
					return false;
				}
			}
		}
		else 
		{
			if(!can_have_subs())
			{
				if(unsigned(v.to_real())==0)
				{
					*result = this;
					return true;
				}
			}
			take_ownership();
			if(!data->retrieve(v,result))
			{
				return false;
				*result = this;
			}
		}
		return true;
	}

	int size()const
	{
		if(!data)return 1;
		return data->size();
	}

	void set_vector()
	{
		clear_data();
		data->type = VT_VEC;
	}

    void set_object(CObjectParser* module)
    {
        clear_data();
        data->type = VT_OBJECT;
        data->object.clear();
        data->object.pModule = module;
    }

	void set_pair(CVarValue const& first,CVarValue const& second)
	{
		clear_data();
		data->type = VT_VEC;
		data->array.resize(2);
		data->array[0]=first;
		data->array[1]=second;
	}

	void set_hash()
	{
		clear_data();
		data->type = VT_HASH;
	}

	void set_hash(CVarValue const& key,CVarValue const& val)
	{
		clear_data();
		data->type = VT_HASH;
		data->hash[key] = val;
	}

	void set_hash(CVarValue const& dta)
	{
		if(dta.type() == VT_HASH){*this = dta;return;}
		clear_data();
		data->type = VT_HASH;
		if(dta.is_nil())return;

		int sz= dta.size();
		for(int i=0;i<sz;i+=2)
		{
			data->hash[dta[i]] = dta[i+1];
		}
	}
	CVarValueMap const& get_hash()const{GSYS_ASSERT(data);return data->hash;}

    void set_hash_value(CVarValue const& key,CVarValue const& val)
    {
        if(!data) set_hash();
        else if(data->type != VT_HASH)
        {
            CVarValue tmp = *this;
            set_hash(tmp);
        }
        data->hash[key] = val;
    }

	void set_sub(SubDef const& sub)
	{
		clear_data();
		data->type = VT_SUB;
		data->sub = sub;
	}

    void set_module(ModuleDef const& sub)
    {
        clear_data();
        data->type = VT_MODULE;
        data->moddef = sub;
    }

    IDCMapModule* get_object_module()
    {
        if(type() != VT_OBJECT)return 0;
        return data->object.object_module();
    }

    void add_object_ref()
    {
        if(type() != VT_OBJECT)return;
        return data->object.add_ref();
    }

	void set_dbref(DBRef const& ref)
	{
		clear_data();
		data->type = VT_DBREF;
		data->dbref = ref;
	}

	void set_dbref(dcmapDBREF const& ref,IDCMapDataStorage* psrc)
	{
		clear_data();
		data->type = VT_DBREF;
		data->dbref.ref = ref.data;
		data->dbref.storage = psrc;
	}

	bool call(CVarValue const& param,CVarValue & res)const
	{
		if(!data)
		{
			res.set_nil();
			return false;
		}
        if(data->type == VT_SUB)
        {
            data->sub.Execute(param,res);
        }
        else if(data->type == VT_MODULE)
        {
            data->moddef.Execute(param,res);
        }
		else if(data->type == VT_SERVICE)
		{
			data->service.Execute(data->str,param,res);
		}        
		else
		{
			res = *this;
			return false;
		}
		return true;
	}

	bool callable()
	{
		if(!data)return false;
		return data->type == VT_SUB || data->type == VT_MODULE || data->type == VT_SERVICE;
	}
};

class CDCMapVarValue: public IDCMapVarValue, public  CVarValue
{
public:
	CDCMapVarValue(CVarValue const& v):CVarValue(v){}
	CDCMapVarValue(){}

	IDCMapVarValue* IFace(){return this;}

    void DCMAPAPI Clear(){set_nil();}
    int  DCMAPAPI Count(){return size();}
    bool DCMAPAPI IsHash(){return type() == VT_HASH;}

    double DCMAPAPI GetNumber(int num=0){return get(num).to_real();}
    dcmapWCSTR DCMAPAPI GetString(int num=0){return get(num).as_string().c_str();}
    dcmapDBREF const* DCMAPAPI GetDBRef(int num=0)
	{
		CVarValue r(get(num));
		if(r.type() != VT_DBREF)return 0;
		return &r.as_dbref();
	}

    double DCMAPAPI GetNamedNumber(dcmapWCSTR name){return get(name).to_real();}
    dcmapWCSTR DCMAPAPI GetNamedString(dcmapWCSTR name){return get(name).as_string().c_str();}
    dcmapDBREF const* DCMAPAPI GetNamedDBRef(dcmapWCSTR name){return &get(name).as_dbref();}

	IDCMapVarValue* DCMAPAPI GetNamedVar(dcmapWCSTR name)
	{
		return new CDCMapVarValue(get(name));
	}
	IDCMapVarValue* DCMAPAPI GetVar(int num=0)
	{
		return new CDCMapVarValue(get(num));
	}

	IDCMapObject* DCMAPAPI GetNamedObject(dcmapWCSTR name)
	{
		CVarValue r(get(name));
		return r.get_sysobject();
	}
	IDCMapObject* DCMAPAPI GetObject(int num=0)
	{
		CVarValue r(get(num));
		return r.get_sysobject();
	}

	void DCMAPAPI SetNumber(int name,double num)
	{
		CVarValue* var;retrieve(CVarValue(name),&var);
		*var = CVarValue(num);
	}

	void DCMAPAPI SetString(int name,dcmapDCSTR string)
	{
		CVarValue* var;retrieve(CVarValue(name),&var);
		*var = CVarValue(string);
	}
	void DCMAPAPI SetDBRef(int name,dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)
	{
		CVarValue* var;retrieve(CVarValue(name),&var);
		if(ref)*var = CVarValue(*ref,pStor);
		else var->set_nil();
	}
	void DCMAPAPI SetVar(int name,IDCMapVarValue* pVar)
	{
		CVarValue* var;retrieve(CVarValue(name),&var);
		if(pVar) *var = *(CDCMapVarValue*)pVar;
		else var->set_nil();
	}

	void DCMAPAPI SetObject(int name,IDCMapObject* pObject)
	{
		CVarValue* var;retrieve(CVarValue(name),&var);
		if(pObject) *var = CVarValue(pObject);
		else var->set_nil();
	}

	void DCMAPAPI SetNamedVar(dcmapWCSTR name,IDCMapVarValue* pVar)
	{
		if(is_nil())set_hash();
		CVarValue* var;retrieve(CVarValue(name),&var);
		if(pVar) *var = *(CDCMapVarValue*)pVar;
		else var->set_nil();
	}

	void DCMAPAPI SetNamedObject(dcmapWCSTR name,IDCMapObject* pObject)
	{
		if(is_nil())set_hash();
		CVarValue* var;retrieve(CVarValue(name),&var);
		if(pObject) *var = CVarValue(pObject);
		else var->set_nil();
	}

    void DCMAPAPI SetNamedNumber(dcmapWCSTR name,double num)
    {
		if(is_nil())set_hash();
        CVarValue* var;retrieve(CVarValue(name),&var);*var = num;
    }
    void DCMAPAPI SetNamedString(dcmapWCSTR name,dcmapDCSTR str)
    {
		if(is_nil())set_hash();
        CVarValue* var;retrieve(CVarValue(name),&var);*var = str;
    }
    void DCMAPAPI SetNamedDBRef(dcmapWCSTR name,dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)
    {
		if(is_nil())set_hash();
        if(!ref)return;
        CVarValue* var;retrieve(CVarValue(name),&var);*var = CVarValue(*ref,pStor);
    }

    void DCMAPAPI AddNumber(double num)
    {
        if(!data)*(CVarValue*)this = num;
        else append(CVarValue(num));
    }
    void DCMAPAPI AddString(dcmapWCSTR str)
    {
        if(!data)*(CVarValue*)this = str;
        else append(CVarValue(str));
    }
    void DCMAPAPI AddDBRef(dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)
    {
        if(!ref)return;
        if(!data)data = new CVarData(*ref,pStor);
        else append(CVarValue(*ref,pStor));
    }

	void DCMAPAPI AddVar(IDCMapVarValue* pVar)
	{
		if(!pVar)return;
		if(!data)*this = *(CDCMapVarValue*)pVar;
		else append(*(CDCMapVarValue*)pVar);
	}

	void DCMAPAPI AddObject(IDCMapObject* pObject)
	{
		if(!pObject)return;
		if(!data)data = new CVarData(pObject);
		else append(CVarData(pObject));
	}

    void DCMAPAPI Destroy(){delete this;}
    IDCMapVarValue* DCMAPAPI MakeCopy()
    {
        return new CDCMapVarValue(*this);
    }

};