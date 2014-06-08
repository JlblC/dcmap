#pragma once

/*

*/

namespace gsys
{
    ///////////////////////////////////////////////////////////////////////////////////////
    class Variable
    {
    public:
		Variable(){m_pData=0;}
        void* m_pData;
    };

    template<class _T> class tVariable : public Variable
    {
    protected:
        _T m_Data;
    public:
        typedef _T value_type;
        _T get(){return m_Data;}
        void set(_T &data){m_Data = data;}
        tVariable()
        {
            m_pData = &m_Data;    
        }
        tVariable(_T data)
        {
            m_pData = &m_Data;    
            m_Data = data;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////////

    class intruded_list : noncopyable
    {
        intruded_list* next;
        intruded_list* prev;
        void remove()
        {GSYS_CFN;
            // release references
            if(next){GSYS_ASSERTOBJ(next);next->prev = prev;}
            if(prev){GSYS_ASSERTOBJ(prev); prev->next = next;}
			next = 0;
			prev = 0;
        }
    public:
        void IntrudedListRemove()
        {GSYS_CFN;
			remove();
		}
        void IntrudedListAssign(intruded_list* list)
        {GSYS_CFN;
            GSYS_ASSERTOBJ(list);
            remove();
            next = list->next;
            prev = list;
            list->next = this;
        }
        intruded_list* IntrudedListNext(void)
        {GSYS_CFN;
            return next;
        }
        intruded_list()
        {GSYS_CFN;
            next = gsys_null;
            prev = gsys_null;
        }
        ~intruded_list()
        {GSYS_CFN;
            remove();    
        }
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    template<class T> class sptr
    {
        T* m_pBuffer;

        sptr()
        {
            m_pBuffer = 0;
        }

        ~sptr()
        {
            Release();
        }

        sptr(sptr& pt)
        {
            m_pBuffer = pt.m_pBuffer;
            (*((int*)m_pBuffer-1))++;
        }

        void New()
        {
            m_pBuffer = gmalloc(sizeof(T)+sizeof(uint)); 
            new(m_pBuffer) T();
        }
        void Release()
        {
            int refcount = (*)--;
            if(refcount == 0)
            {
                delete(m_pBuffer) 
                    gfree((int*)m_pBuffer-1);
            }
        }
    }; 
    ///////////////////////////////////////////////////////////////////////////////////////
    // template class for simplifing memory reading writing
    template<class T> class tMemStream
    {
    protected:
        T* m_pData;
        T* m_pPointer;
        bool m_bOwnHandle;
        int m_iCount;
    public:
        typedef T value_type; 

        tMemStream()
        {
            m_iCount = 0;
            m_pData = gsys_null;        
            m_pPointer = gsys_null;
            m_bOwnHandle = false;
        }
        ~tMemStream()
        {
            Free();
        }

        void Init(int count)
        {
            m_iCount = count;
            m_pData = new T[count];        
            m_pPointer = m_pData;
            m_bOwnHandle = true;
        }

        void Free()
        {
            if(m_bOwnHandle)
                delete m_pData;
        }

        void Assign(T* pt, int count)
        {
            m_pData = pt;
            m_pPointer = pt;
            m_bOwnHandle = false;
            m_iCount = count;
        }
        void operator << (const T &dta)
        {
            *m_pPointer = dta;
            m_pPointer++;
        }
        void operator >> (T &dta)
        {
            dta = *m_pPointer;
            m_pPointer++;
        }
    };

	class MemCollector
    {
        gsys_byte* m_pBegin;
        gsys_byte* m_pPointer;
        gsys_byte* m_pEnd;

		static const alloc_base = 512;
		static const alloc_add = 256;
    public:
 		typedef gsys_ulong size_type;
       MemCollector()
        {
            m_pBegin = 0;
            m_pPointer = 0;        
            m_pEnd = 0;
        }
        ~MemCollector()
        {
            if(m_pBegin) gfree(m_pBegin);
        }
		size_type size(){return size_type(m_pPointer-m_pBegin);}
		gsys_byte const* data(){return m_pBegin;}
		size_type capacity(){return size_type(m_pEnd-m_pBegin);}
		void clear()
		{
			if(m_pBegin) gfree(m_pBegin);
            m_pBegin = 0;
            m_pPointer = 0;        
            m_pEnd = 0;
		}

		void Write(void const* data, size_type data_size)
		{
			if(!m_pPointer) // alloc new data data buffer
			{
				ulong newsz = max(ulong(alloc_base),ulong(data_size))+alloc_add;
				m_pBegin = (gsys_byte*)galloc(newsz);
				m_pPointer = m_pBegin;
				m_pEnd = m_pBegin+newsz;
			}
			else if( (m_pPointer+data_size) > m_pEnd) // realloc data buffer
			{
				GSYS_ASSERTOBJ(m_pPointer);
				GSYS_ASSERTOBJ(m_pEnd);
				
				ulong oldsz = size();
				ulong newsz = max(ulong(capacity()+alloc_add),ulong(size()+data_size))+alloc_add;
				m_pBegin = (gsys_byte*)grealloc(m_pBegin,newsz);
				m_pPointer = m_pBegin+oldsz;
				m_pEnd = m_pBegin+newsz;
			}
			memcpy(m_pPointer,data,data_size);
            m_pPointer+=data_size;
		}

		template<class T>
        void operator << (T const& dta)
        {
			Write(&dta,sizeof(dta));
        }
    };

	template<typename T>
    class indexed_tree
    {
        class entry;
    public:
		typedef typename std::vector<entry>::size_type index_type; 
    private:
        class entry
        {
        public:
            T data;
            index_type next;
            index_type prev;
            index_type child;
            index_type parent;
        };
        class _iterator
        {
        friend class indexed_tree;
        protected:
            std::vector<entry> * m_pvcEntries;
            index_type m_iIndex;
/*            _iterator(std::vector<entry> *entries,index_type _Index)
            {
                m_iIndex = _Index;
                m_pvcEntries = entries;
            }
*/
            index_type& next_index()const
            {return (*m_pvcEntries)[m_iIndex].next;}
            index_type& prev_index()const
            {return (*m_pvcEntries)[m_iIndex].prev;}
            index_type& child_index()const
            {return (*m_pvcEntries)[m_iIndex].child;}
            index_type& parent_index()const
            {return (*m_pvcEntries)[m_iIndex].parent;}

            // primary for debug
            T const& data()const
            {return (*m_pvcEntries)[m_iIndex].data;}
        public:
            operator bool()const{return m_pvcEntries != gsys_null;}
            bool next()
            {
                if(!(*m_pvcEntries)[m_iIndex].next)return false;
                m_iIndex = (*m_pvcEntries)[m_iIndex].next;
                return true;
            }
            index_type index()const{return m_iIndex;}
        };
   public:
       class iterator : public _iterator
       {
        friend class indexed_tree;
       public:
            T* operator ->(){return &((*m_pvcEntries)[m_iIndex].data);}
            T& operator *(){return((*m_pvcEntries)[m_iIndex].data);}
            iterator childs()const
            {
                if(!(*m_pvcEntries)[m_iIndex].child)return iterator(gsys_null,0);
                return iterator(m_pvcEntries,(*m_pvcEntries)[m_iIndex].child);
            }
            iterator(std::vector<entry> *entries,index_type _Index)
            {
                m_iIndex = _Index;
                m_pvcEntries = entries;
            }
       };
       class const_iterator : public _iterator
       {
        friend class indexed_tree;
       public:
            T const* operator ->(){return &((*m_pvcEntries)[m_iIndex].data);}
            T const& operator *(){return(*m_pvcEntries)[m_iIndex].data;}
            const_iterator childs()const
            {
                if(!(*m_pvcEntries)[m_iIndex].child)return const_iterator(gsys_null,0);
                return const_iterator(m_pvcEntries,(*m_pvcEntries)[m_iIndex].child);
            }
            const_iterator(std::vector<entry> const* entries,index_type _Index)
            {
                m_iIndex = _Index;
                m_pvcEntries = const_cast<std::vector<entry>*>(entries);
            }
       };
    private:
       std::vector<entry> m_vcEntries;

       iterator iterator_at(index_type _Index){return iterator(&m_vcEntries,_Index);}
       entry& entry_at(index_type _Index){return m_vcEntries[_Index];}

        index_type add_entry(entry const& ent)
        {
            // TODO: add support for filing deleted entries
            m_vcEntries.push_back(ent);
            return m_vcEntries.size()-1;
        }
    public:
       indexed_tree(indexed_tree const& ind)
       {
           m_vcEntries = ind.m_vcEntries;          
       }
       indexed_tree()
       {
           entry ent;
           ent.child = ent.next = ent.parent = ent.prev = 0;
           m_vcEntries.push_back(ent); // add root element     
       }
       index_type size()const{return m_vcEntries.size()-1;}
       void clear()
       {
           m_vcEntries.erase(m_vcEntries.begin()+1,m_vcEntries.end());     
       }

       iterator root(){return iterator(&m_vcEntries,0);}
       const_iterator root()const{return const_iterator(&m_vcEntries,0);}

       iterator child(index_type _Index)
            {return iterator(&m_vcEntries, m_vcEntries[_Index].child);}
       const_iterator child(index_type _Index)const
            {return const_iterator(&m_vcEntries, m_vcEntries[_Index].child);}

       T& operator [](index_type pos){return m_vcEntries[pos].data;}   
       T const& operator [](index_type pos)const{return m_vcEntries[pos].data;} 

       iterator insert(iterator const& position,T const& data)
       {
            index_type _Index;
            entry ent;
            ent.data = data;
            ent.next = position.next_index();
            ent.prev = position.index();
            ent.parent = position.parent_index();
            ent.child = 0;
            _Index = add_entry(ent);
            position.next_index() = _Index;
            entry_at(position.next_index()).prev = _Index;
            return iterator_at(_Index);
       }

       iterator insert_child(iterator const& position,T const& data)
       {
           GSYS_CFN;
           if(position.child_index())
               return insert(position.childs(),data);

           entry ent;
           ent.data = data;
           ent.parent = position.index();
           ent.next = 0;
           ent.prev = 0;
           ent.child = 0;
           position.child_index() = add_entry(ent);
           return iterator_at(position.child_index());
       }
    };

}