#pragma once
#ifndef G_SMARTPTR_REFCOUNT_H
#define G_SMARTPTR_REFCOUNT_H

#include "g_gsys.h"

namespace gsys
{
    template<class T>class smart_ref_ptr_base;
    template<class T>class srptr;
    template<class T>class const_srptr;

	/*
   class smart_ref_base{
        friend class smart_ref_ptr_base;
        int m_iRef;
    protected:
        void AddRef(){m_iRef++;}
        bool Release(){m_iRef--;if(!m_iRef)return OnDestroy();}
        virtual bool OnDestroy()=0;
    public:
        virtual ~smart_ref_base(){};
        smart_ref_base(){
            m_iRef=0;
        }
    };   

    class ref_int
    {
        friend smart_ref_ptr_base;
        int m_iRef;
        void AddRef(){m_iRef++;}
        bool Release(){m_iRef--;if(m_iRef<=0)return true;return false;}
    public:
		ref_int():m_iRef(0){}
		virtual ~ref_int(){}
    };
    class controlled_ref_int
    {
        friend class smart_ref_ptr_base;
    protected:
        int m_iRef;
        void AddRef(){m_iRef++;}
        bool Release(){m_iRef--;if(m_iRef<=0)return true;return false;}
    public:
		void FreeControlledObject()
		{
			if(Release())
				delete this;
		}
		bool ControlledObjectUsed()
		{
			return m_iRef > 1;
		}

        controlled_ref_int(){m_iRef=1;}
		virtual ~controlled_ref_int(){}
    };
	*/

    template<class T> class smart_ref_ptr_base{
    protected:
        T* m_ptr;
        void release()
		{
            if(m_ptr){
                GSYS_ASSERTOBJ(m_ptr);
                if(m_ptr->Release())
                {
                    delete m_ptr;
					m_ptr = 0;
                }
            }
        }
        void set(T* pt)
		{GSYS_CFN;GSYS_ASSERTOBJ_NULL(pt);
            m_ptr=pt;
            if(m_ptr){
                GSYS_ASSERTOBJ(m_ptr);
                m_ptr->AddRef();
            }
        }
    public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef T const& const_reference;
		typedef T const* const_pointer;

        template<class _NT> 
		void _new(_NT* newpt)
        {

		}

        template<class _NT> 
		void _new()
        {
			release();
            set(new _NT());
		}

		void _new()
		{
			release();
			set(new T());
		}

        pointer get()const{GSYS_CFN;GSYS_ASSERTOBJ(m_ptr);return m_ptr;}
        smart_ref_ptr_base(){m_ptr=0;}
        //smart_ref_ptr_base(T* pt) {set(pt);}
//        smart_ref_ptr_base(smart_ref_ptr_base<T>const& ptr) {set(ptr.m_ptr);}
        ~smart_ref_ptr_base() {release();}
//        operator =(smart_ref_ptr_base<T>const& ptr){release();set(ptr.m_ptr);}
        //T* operator ->(){GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        // validness
        operator bool() const { return m_ptr != 0;}

		void reset(){release();m_ptr=gsys_null;}

        //operator T*() const {return m_ptr;}
    }; 

    template<class T> class srptr: public smart_ref_ptr_base<T>
    {
    friend class const_srptr<T>;
    public:
        pointer operator ->()const{GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        reference operator *()const{GSYS_ASSERTOBJ(m_ptr);return *m_ptr;} 

        srptr(){};
        srptr(T* pt) {set(pt);}
        srptr(srptr<T>const& ptr) {GSYS_ASSERTREF(ptr);set(ptr.m_ptr);}

        template<class CT> srptr<CT> cast(void)
        {
            return srptr<CT>((CT*)(m_ptr));    
        }

        srptr<T> & operator =(srptr<T>const& ptr){release();set(ptr.m_ptr);return *this;}
        srptr<T> & operator =(T* ptr){release();set(ptr);return *this;}
    };

    template<class T> class const_srptr : public smart_ref_ptr_base<T>
    {
    public:
        const_pointer operator ->()const{GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        const_reference operator *()const{GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        const_srptr<T>& operator =(srptr<T>const& ptr){release();set(ptr.m_ptr);return *this;}
        const_srptr<T>& operator =(const_srptr<T>const& ptr){release();set(ptr.m_ptr);return *this;}

        const_srptr(){};
        const_srptr(T* pt) {set(pt);}
        const_srptr(srptr<T>const& ptr) {set(ptr.m_ptr);}
        const_srptr(const_srptr<T>const& ptr) {set(ptr.m_ptr);}
    };


	template<class T> 
	class objptr_refcnt 
    {
	public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef T const& const_reference;
		typedef T const* const_pointer;
	protected:
		pointer m_ptr;

		bool _reset(pointer ptr)
		{
			if(m_ptr == ptr)return false;
			if(m_ptr)m_ptr->Release();
			m_ptr = ptr;
			return (bool)m_ptr;
		}
	public:

		objptr_refcnt():m_ptr(0){}
		objptr_refcnt(pointer obj):m_ptr(obj){}
		objptr_refcnt(pointer obj, bool add_ref):m_ptr(obj)
		{
			if(add_ref&&m_ptr)m_ptr->AddRef();
		}

		objptr_refcnt(objptr_refcnt<T>const& ptr):m_ptr(ptr.m_ptr)
		{
			if(m_ptr)m_ptr->AddRef();
		}

		objptr_refcnt<T>& operator =(objptr_refcnt<T>const& ptr)
		{
			assign(ptr.m_ptr);
			return *this;
		}

		objptr_refcnt<T>& operator =(pointer ptr)
		{
			reset(ptr);
			return *this;
		}

		void reset(pointer ptr, bool add_ref=false)
		{
			if(m_ptr)m_ptr->Release();
			if(add_ref&&ptr)ptr->AddRef();
			m_ptr = ptr;
		}

		template<class PtrT> 
		void reset_dcast(PtrT* ptr, bool add_ref=false)
		{
			reset(dynamic_cast<pointer>(ptr),add_ref);
		}

		void assign(pointer ptr)
		{
			if(_reset(ptr))ptr->AddRef();
		}
		void swap(objptr_refcnt<T>& v)
		{
			gsys::exchange(m_ptr,v.m_ptr);
		}

		~objptr_refcnt()
		{
			if(m_ptr)m_ptr->Release();
		}

		void free()
		{
			reset(gsys_null);
		}

		void free_delete()
		{
			if(m_ptr)
			{
				if(!m_ptr->Release())
				{
					delete m_ptr;
				}
				m_ptr = gsys_null;
			}
		}

		void _delete ()
		{
			if(m_ptr)
			{
				delete m_ptr;				
				m_ptr = 0;
			}			
		}

        pointer get()const{GSYS_ASSERTOBJ_NULL(m_ptr);return m_ptr;} 

        pointer operator ->(){GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        reference operator *(){GSYS_ASSERTOBJ(m_ptr);return *m_ptr;} 
        
		const_pointer operator ->()const{GSYS_ASSERTOBJ(m_ptr);return m_ptr;} 
        const_reference operator *()const{GSYS_ASSERTOBJ(m_ptr);return *m_ptr;} 
      
		bool operator < (objptr_refcnt<T> const& ptr)const{return m_ptr < ptr.m_ptr;}
		bool operator > (objptr_refcnt<T> const& ptr)const{return m_ptr > ptr.m_ptr;}
		bool operator == (objptr_refcnt<T> const& ptr)const{return m_ptr == ptr.m_ptr;}

		bool valid()const{return m_ptr != 0;}
		operator bool()const{return m_ptr != 0;}
	};
}
#endif