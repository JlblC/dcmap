#pragma once

#include "gsys.h"
#include "gmem.h"
#include <vector>

namespace gsys {
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
};