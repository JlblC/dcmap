#pragma once

namespace gsys
{
	template<class T>
	class ref_ptr
	{
	public: // typedefs
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef T const* const_pointer;
		typedef T const& const_reference;
	private: // data members
		pointer m_pRef;
		bool m_bOwnHandle;
	public: // construction destructions
		ref_ptr():m_pRef(0),m_bOwnHandle(false){}
		ref_ptr(pointer pt):m_pRef(pt),m_bOwnHandle(false){}
		ref_ptr(ref_ptr<value_type> const& pt):m_pRef(pt.m_pRef),m_bOwnHandle(false){}
		~ref_ptr(){reset();}
		ref_ptr<value_type> & operator = (ref_ptr<value_type> const& pt)
		{
			reset();
			m_bOwnHandle = false;
			m_pRef = pt.m_pRef;
			return *this;
		}
		void reset(){if(m_bOwnHandle)delete m_pRef;m_pRef=0;}
		void assign_new(pointer pt){reset();m_pRef = pt;m_bOwnHandle=true;}
	public:
		pointer get(){GSYS_ASSERT(m_pRef);return m_pRef;}
		reference operator *(){GSYS_ASSERT(m_pRef);return *m_pRef;}
		pointer operator ->(){GSYS_ASSERT(m_pRef);return m_pRef;}
		operator bool(){return bool(m_pRef != gsys_null);}
	};

}
