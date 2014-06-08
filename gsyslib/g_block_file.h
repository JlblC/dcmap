#pragma once

#include "g_gsys.h"
#include "g_file.h"
#include "g_gid.h"
#include "g_crc.h"
#include "g_data_buffer.h"
#include <map>

#include "g_zlib.h"

namespace gsys {
////////////////////////////////////////////////////////////////////////////////////

class CBlockFile
{
public:
	typedef uint size_type;
protected:
	static const GID CBlockFile_CRC32BlockGID;
	static const GID CBlockFile_FilnalBlockGID;
	static const GID CBlockFile_CompressedHeaderBlockGID;

	static const MaxSectionSize = 1024*1024*5; // 5 mb // need 10mb for processing 

	crc32_counter m_crc; // crc32 counter class
	struct BlockHeader
	{
		GID gidBlockClass;
		ID	idBlock;
		int iRecordSize;
		int iRecordCount;
	};
	struct FinalBlock
	{
		ID idRootBlock;
        GID gidFile;
		ID idLastId;
	};
	struct CompressedBlock
	{
		int iSize; // Size of struct for futher compatiblity
		ulong DataSize;
 		ulong CompressedSize;
		word   Compressor; // 0 - zlib any other value - unsupported block
	//	word   CompressorParams;
	};

};

////////////////////////////////////////////////////////////////////////////////////

class CBlockFileReader : public CBlockFile
{
protected:
    struct MemId
    {
        BlockHeader head;
        void* pdata;
    };
    typedef std::map<ID,MemId> MemIdMap;
    MemIdMap m_mapData;
    FinalBlock m_FinalBlock;
	bool m_bErrorState;

	int fRead(gfile* file,void* pBuffer, size_type iSize,size_type iCount=1)
	{GSYS_CFN;
		GSYS_ASSERTOBJ(file);
		int r = file->read(pBuffer,iSize*iCount);
		if(!r)m_bErrorState = true;
		m_crc.count(pBuffer,iSize*iCount);
		return r;
	}
	bool ProcessCompressedFile(gfile * file,size_type len,size_type compr_len)
	{GSYS_CFN;
		gsys_byte* buf = new gsys_byte[compr_len];
		size_type rsz = fRead(file,buf,compr_len);
		gsys_byte* Buffer = new gsys_byte[len];
		uLongf uncsz = len;
		int r = uncompress(Buffer,&uncsz,buf,compr_len);
		delete[] buf;
		gsys_byte* ptr = Buffer;
		gsys_byte* endptr = Buffer+len;
		// TODO: Check Integrity
		// parce uncompressed data
		MemId mid;
        while(ptr < endptr)
        {
			memcpy(&mid.head,ptr,sizeof(mid.head));ptr += sizeof(mid.head);
			const int sz = mid.head.iRecordCount*mid.head.iRecordSize;
			mid.pdata = galloc(sz);
			memcpy(mid.pdata,ptr,sz);ptr += sz;
			m_mapData[mid.head.idBlock] = mid;
		}
		delete[] Buffer;
		return false;
	}
public:
    ~CBlockFileReader()
    {GSYS_CFN;
        Clear();
    }
    CBlockFileReader()
    {GSYS_CFN;
        m_bErrorState = false;
    }

    ID ProcessFile(gfile* file)
    {GSYS_CFN;
		m_bErrorState = false;
		m_crc.clear();
        Clear();
        MemId mid;
        for(;;)
        {
            fRead(file,&mid.head,sizeof(mid.head)); 
            const int sz = mid.head.iRecordCount*mid.head.iRecordSize;
			if(sz > MaxSectionSize || m_bErrorState) // error in input data
			{
				Clear();
				m_FinalBlock.idRootBlock = ID();
				return ID();
			}
            if(mid.head.gidBlockClass == CBlockFile_FilnalBlockGID) // Stop Reading blocks
            {
				mid.pdata = galloc(sz);
				fRead(file,mid.pdata,sz);
				m_mapData[mid.head.idBlock] = mid;
                m_FinalBlock = *((FinalBlock*)mid.pdata);
				crc_t crc = m_crc.crc();
				crc_t stcrc = 0;
				fRead(file,&mid.head,sizeof(mid.head)); 
				if(mid.head.gidBlockClass == CBlockFile_CRC32BlockGID)
					fRead(file,&stcrc,sizeof(stcrc));
				if(1) // check crc !!!!!!!
				{
					if(crc != stcrc || m_bErrorState)
					{	
						Clear();
						m_FinalBlock.idRootBlock = ID();
						return ID();
					}
				}
                break;
            }
			else if(mid.head.gidBlockClass == CBlockFile_CompressedHeaderBlockGID) // Read compressed blocks
			{
				CompressedBlock block;
				int & block_size = block.iSize;
				memset(&block,0,sizeof(block));
				fRead(file,&block_size,sizeof(block_size));;
				fRead(file,((gsys_byte*)(&block))+sizeof(block_size),min(size_type(sizeof(CompressedBlock)-sizeof(block_size)),size_type(block_size)-sizeof(block_size)));
				if(block_size > sizeof(block))file->seek(block_size - sizeof(block),SEEK_CUR);
				
				if(block.Compressor != 0) // scip unknown compressor
				{
					gsys_byte *buf = new gsys_byte[block.CompressedSize];
					fRead(file,buf,block.CompressedSize);
					delete[] buf;
				}
				else 
				{
					ProcessCompressedFile(file,block.DataSize,block.CompressedSize);
				}
			}
			else
			{
				mid.pdata = galloc(sz);
				fRead(file,mid.pdata,sz);
				m_mapData[mid.head.idBlock] = mid;
			}
        }
        return m_FinalBlock.idRootBlock;
    }

    ID GetRootId()const
    {GSYS_CFN;
        return m_FinalBlock.idRootBlock;
    }
    GID GetFileGID()const
    {GSYS_CFN;
        return m_FinalBlock.gidFile;    
    }

    void Clear()
    {GSYS_CFN;
        MemIdMap::iterator iter = m_mapData.begin();
        while(iter !=  m_mapData.end())
        {
            gfree(iter->second.pdata);
            iter++;
        }
        m_mapData.clear();    
    }

    int GetSize(ID idBlock)const
    {GSYS_CFN;
		MemIdMap::const_iterator iter = m_mapData.find(idBlock);
        if(iter ==  m_mapData.end()) return 0;
        MemId mid = iter->second;
        return mid.head.iRecordSize*mid.head.iRecordCount;
    }

    GID GetGID(ID idBlock)const
    {GSYS_CFN;
        MemIdMap::const_iterator iter = m_mapData.find(idBlock);
        if(iter ==  m_mapData.end()) return InvalidGID;
        MemId mid = iter->second;
        return mid.head.gidBlockClass;
    }

    void* GetDataPt(ID idBlock,size_type &size, size_type &count)const
    {GSYS_CFN;
		if(!idBlock)return gsys_null;
        MemIdMap::const_iterator iter = m_mapData.find(idBlock);
        if(iter ==  m_mapData.end()) return gsys_null;
        MemId mid = iter->second;
        size = mid.head.iRecordSize;
        count = mid.head.iRecordCount;
        return mid.pdata;
    }

    std::string GetString(ID idBlock)const
    {GSYS_CFN;
        size_type sz,cnt;
        char* pt = (char*)GetDataPt(idBlock,sz,cnt);
		if(!pt) return "";
		GSYS_ASSERTPTR(pt);
        if(sz != sizeof(char)) return "";
        return std::string(pt,cnt);
    }

    bool GetBuffer(ID idBlock,void* buf,size_type size,size_type count=1)const
    {GSYS_CFN;
        size_type sz,cnt;
        void* pt = GetDataPt(idBlock,sz,cnt);
        if(!pt)return false;
		GSYS_ASSERTPTR(pt);
		size_type msz = min(size,sz);
        if(sz == size && cnt == count)
        {
            memcpy(buf,pt,sz*cnt);
            return true;
        }
		else if(cnt == 1) // copy aviable part
		{
            memcpy(buf,pt,msz);
            return true;
		}
		else // copy aviable parts
		{
			size_type mcnt = min(cnt,count);
			gsys_byte* l_pt = (gsys_byte*)pt;
			gsys_byte* l_dpt = (gsys_byte*)buf;
			for(size_type i=0;i<mcnt;i++,l_pt+=sz,l_dpt+=size)
			{
				memcpy(l_dpt,l_pt,msz);			
			}
		}
        return true;
    }

    template<class T> bool tFillVector(ID idBlock,std::vector<T> & vt)const
    {GSYS_CFN;
        size_type sz,cnt;
        void* pt = GetDataPt(idBlock,sz,cnt);
        if(!pt)return false;
		GSYS_ASSERTPTR(pt);
		size_type msz = min(sz,sizeof(T));
        if(sz == sizeof(T))
		{
			T* tpt = (T*)pt;
			for(size_type i=0;i<cnt;i++)
			{
				vt.push_back(tpt[i]);
			}
		}
		else
		{
			gsys_byte* l_pt = (gsys_byte*)pt;
			for(size_type i=0;i<cnt;i++,l_pt+=sz)
			{
				T l_t;
				memcpy(&l_t,l_pt,msz);			
				vt.push_back(l_t);
			}
		}
        return true;
    }

    template<class T> bool tGetBuffer(ID idBlock,T* buf,size_type count=1)const
    {GSYS_CFN;
        return GetBuffer(idBlock,buf,sizeof(T),count);
    }
};

class CBlockFileWriter : public CBlockFile
{
protected:
	gfile* m_pFile;
	raw_data_buffer<gsys_byte> m_buffer;
	ID	   m_idCurrentId;
	bool   m_bCompressing;
	int    m_iCompress;
	ulong  m_ulMaxCompressSize;
	ulong  m_ulComprSize;

	void ofWrite(BlockHeader &head,void* pvdata)
	{GSYS_CFN;
		GSYS_ASSERTREF(head);
		if(!m_iCompress)
		{
			fWrite(&head,sizeof(head));
			if(pvdata)fWrite(pvdata,head.iRecordSize,head.iRecordCount);
		}
		else // Write to compress collector
		{
			GSYS_ASSERT(pvdata);
			m_bCompressing=true;
			ulong sz = head.iRecordCount*head.iRecordSize;
			m_buffer.append((gsys_byte*)&head,sizeof(head));
			m_buffer.append((gsys_byte*)pvdata,sz);
			if(m_buffer.size() >= m_ulMaxCompressSize)
				FinalizeCompressing();
		}		
	}
	void FinalizeCompressing()
	{GSYS_CFN;
		GSYS_ASSERT(m_bCompressing);
		GSYS_ASSERT(m_iCompress);
		m_bCompressing = false;

		CompressedBlock cb;
		cb.iSize = sizeof(CompressedBlock);
		cb.Compressor = 0;
		cb.DataSize = m_buffer.size();

		uLongf destLen = ulong(float(cb.DataSize)*1.011f)+12;
		gsys_byte *dst = new gsys_byte[destLen];
		compress2(dst,&destLen,m_buffer.get(),m_buffer.size(),m_iCompress);
		m_iCompress = 0;
		cb.CompressedSize = destLen;

		BlockHeader head;
		head.gidBlockClass = CBlockFile_CompressedHeaderBlockGID;
		head.idBlock = m_idCurrentId;
		head.iRecordSize = destLen+sizeof(cb);
		head.iRecordCount = 1;
		ofWrite(head,0);
		m_idCurrentId++;

		fWrite(&cb,sizeof(cb));
		fWrite(dst,destLen);

		m_buffer.reset();
		delete[] dst;
	}
	int fWrite(void* pBuffer, int iSize,int iCount=1)
	{GSYS_CFN;
		GSYS_ASSERTOBJ(m_pFile);
		m_crc.count(pBuffer,iSize*iCount);
		return m_pFile->write(pBuffer,iSize,iCount);
	}
public:
	CBlockFileWriter() : m_idCurrentId(1)
	{GSYS_CFN;
		m_bCompressing = false;
		m_pFile=0;
		m_ulMaxCompressSize = 1024*1024;
		m_ulComprSize=0;
	}
	CBlockFileWriter(gfile* pFile,int comprssion = 9)
	{GSYS_CFN;
		m_bCompressing = false;
		m_pFile=0;
		m_ulMaxCompressSize = 1024*1024;
		m_ulComprSize=0;
		AssignFile(pFile,comprssion);
	}
	void SetComression(int comprssion)
	{GSYS_CFN;
		if(m_bCompressing)FinalizeCompressing();
		m_iCompress = comprssion;
	}
	void AssignFile(gfile* pFile,int comprssion = 9) // comprssion must be betwin 0 and 9
	{GSYS_CFN;
		m_idCurrentId = 1;
		GSYS_ASSERT(!pFile || *pFile);
		m_iCompress = comprssion;
		m_pFile = pFile;
		m_crc.clear();
	}
	gfile* ReleaseFile()
	{GSYS_CFN;
		gfile* pOldFile = m_pFile;
		m_pFile = 0;
		return pOldFile;
	}
	void Finalize(ID idRootBlock,GID gidFile = GeneralGID)
	{GSYS_CFN;
		GSYS_CFN;
		if(m_bCompressing)FinalizeCompressing();
		FinalBlock block;
		block.idRootBlock = idRootBlock;
        block.gidFile = gidFile;
		block.idLastId = m_idCurrentId+4; // next safe id 
		tWriteBlock(&block,CBlockFile_FilnalBlockGID);
		crc_t crc = m_crc.crc();
		tWriteBlock(&crc,CBlockFile_CRC32BlockGID); // store CRC32 in optional block

		// free memory
		m_buffer.clear();
	};
	ID WriteBlock(void* pvdata,int iSize,const GID &gid=GID(),size_type iCount=1)
	{GSYS_CFN;
		if(iCount == 0 || iSize == 0 || pvdata == 0) return ID();
		BlockHeader head;
		head.gidBlockClass = gid;
		head.idBlock = m_idCurrentId;
		head.iRecordSize = iSize;
		head.iRecordCount = iCount;
		ofWrite(head,pvdata);
		return m_idCurrentId++;
	}
    // Common overloaders
/*  Currently no need of gid buffer
    ID Write(const gidbuffer &buf)
    {
        return WriteBlock(buf.data(),buf.size(),buf.gid());    
    }
*/
    template<class T> ID WriteString(std::basic_string<T> const& str,const GID &gid=GID())
    {GSYS_CFN;
        return tWriteBlock(str.c_str(),gid,(size_type)str.size());   
    }

	template<class T> ID tWriteBlock(T* pData,const GID &gid=GID(),size_type iCount=1)
	{GSYS_CFN;
		return WriteBlock((void*)pData,sizeof(T),gid,iCount);
	}
	// vrites entire vector<> using is's data
    template<class T> ID tWriteVectorBlock(const std::vector<T> &vecData,const GID &gid=GID())
	{GSYS_CFN;
		if(vecData.size() == 0)return ID();
		return tWriteBlock(&(*vecData.begin()),gid,(int)vecData.size());		
	}
};

};//namespace gsys 