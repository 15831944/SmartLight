// FlatStorage.h: interface for the CFlatStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLATSTORAGE_H__4EF56DD4_5674_444F_A9EC_01E0401C2E74__INCLUDED_)
#define AFX_FLATSTORAGE_H__4EF56DD4_5674_444F_A9EC_01E0401C2E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory.h>

template<class TYPE>
class CFlatStorage  
{
public:
	CFlatStorage(unsigned int records_in_block = 4096,
		unsigned long max_records = 0x7FFFFFL);
	
	virtual ~CFlatStorage();
	
protected:
	#define FS_MAX_LEVELS	4		/* 128^5 records is enough */
	#define FS_PTRS_IN_NOD	128
	
	typedef struct 
	{
		TYPE* blocks[FS_PTRS_IN_NOD];		/* pointers to HP_PTRS or records */
	}DPTR;
	
	typedef struct
	{
		unsigned int free_ptrs_in_block;
		unsigned int records_under_level;
		DPTR *last_blocks;			/* pointers to HP_PTRS or records */
	}DLEVEL;
	
	
	DPTR *m_root;
	DLEVEL m_level_info[FS_MAX_LEVELS+1];
	unsigned int m_levels;
	unsigned int m_records_in_block;		/* Records in a heap-block */
	unsigned int m_max_records;
	
public:
	TYPE& operator[] (unsigned int pos) const;
	int SetAt(unsigned int pos, TYPE* pRecord);

	int GetNewBlock(unsigned long *alloc_length);
	unsigned char *FreeLevel(unsigned int level, DPTR *pos, unsigned char *last_pos);
};


template<class TYPE>
CFlatStorage<TYPE>::CFlatStorage(unsigned int records_in_block,
						   unsigned long max_records)
{
	unsigned int i;
	
	m_root = 0;
	m_levels = 0;
	memset(m_level_info, 0, sizeof(m_level_info));

	m_max_records = max_records;
	
	//recbuffer =(unsigned int) (reclength + sizeof(unsigned char**)-1) & ~(sizeof(unsigned char**)-1);
	if(!records_in_block)
		records_in_block = 128;
	
	m_records_in_block = records_in_block;
	
	for (i=0 ; i <= FS_MAX_LEVELS ; i++)
		m_level_info[i].records_under_level=
		(!i ? 1 : i == 1 ? records_in_block :
	FS_PTRS_IN_NOD * m_level_info[i-1].records_under_level);

	unsigned long len;
	GetNewBlock(&len);
}

template<class TYPE>
CFlatStorage<TYPE>::~CFlatStorage()
{
	if(m_levels)
		FreeLevel(m_levels, m_root, 0);
}

template<class TYPE>
TYPE& CFlatStorage<TYPE>::operator[] (unsigned int pos) const
{
	int i;
	DPTR *ptr;
	
	for (i=m_levels-1, ptr = m_root ; i > 0 ; i--)
	{
		ptr=(DPTR*)ptr->blocks[pos/m_level_info[i].records_under_level];
		pos%=m_level_info[i].records_under_level;
	}

	return *(TYPE*)((unsigned char*) ptr+ pos * sizeof(TYPE));
}

template<class TYPE>
int CFlatStorage<TYPE>::GetNewBlock(unsigned long *alloc_length)
{
	unsigned int i,j;
	DPTR *root;
	
	for (i=0 ; i < m_levels ; i++)
		if (m_level_info[i].free_ptrs_in_block)
			break;
		
	*alloc_length=sizeof(DPTR)*i+m_records_in_block* sizeof(TYPE);

	if (!(root=(DPTR*) new unsigned char[*alloc_length]))
		return 1;

	if (i == 0)
	{
		m_levels=1;
		m_root=m_level_info[0].last_blocks=root;
	}
	else
	{
		if ((unsigned int) i == m_levels)
		{
			m_levels=i+1;
			m_level_info[i].free_ptrs_in_block = FS_PTRS_IN_NOD-1;
			((DPTR**) root)[0] = m_root;
			m_root = m_level_info[i].last_blocks = root++;
		}
		m_level_info[i].last_blocks->
			blocks[FS_PTRS_IN_NOD - m_level_info[i].free_ptrs_in_block--]=
			(TYPE*) root;
		
		for (j=i-1 ; j >0 ; j--)
		{
			m_level_info[j].last_blocks= root++;
			m_level_info[j].last_blocks->blocks[0]=(TYPE*) root;
			m_level_info[j].free_ptrs_in_block=FS_PTRS_IN_NOD-1;
		}
		m_level_info[0].last_blocks= root;
	}
	return 0;
}

template<class TYPE>
int CFlatStorage<TYPE>::SetAt(unsigned int pos, TYPE* pRecord)
{
	
}

template<class TYPE>
unsigned char *CFlatStorage<TYPE>::FreeLevel(unsigned int level, DPTR *pos, unsigned char *last_pos)
{
	int i,max_pos;
	unsigned char *next_ptr;
	
	if (level == 1)
		next_ptr=(unsigned char*) pos + sizeof(TYPE);
	else
	{
		max_pos = (m_level_info[level-1].last_blocks == pos) ?
			FS_PTRS_IN_NOD - m_level_info[level-1].free_ptrs_in_block :
		FS_PTRS_IN_NOD;
		
		next_ptr=(unsigned char*) (pos+1);
		for (i=0 ; i < max_pos ; i++)
			next_ptr=FreeLevel(level - 1,
			(DPTR*)pos->blocks[i],next_ptr);
	}
	if ((unsigned char*) pos != last_pos)
	{
		delete[] pos;
		return last_pos;
	}
	
	return next_ptr;			/* next memory position */
}

//	/* Returns ptr to block, and allocates block if neaded */
//
//template<class TYPE>
//unsigned char *CFlatStorage<TYPE>::next_free_record_pos()
//{
//	return 0;
////
////  int block_pos;
////  byte *pos;
////  ulong length;
//// 
////  if (!(block_pos=(this->m_P % info->block.records_in_block)))
////  {
////    if (info->records > info->max_records && info->max_records)
////    {
////      my_errno=HA_ERR_RECORD_FILE_FULL;
////      DBUG_RETURN(NULL);
////    }
////    if (_hp_get_new_block(&info->block,&length))
////      DBUG_RETURN(NULL);
////    info->data_length+=length;
////  }
////  DBUG_PRINT("exit",("Used new position: %lx",
////		     (byte*) info->block.level_info[0].last_blocks+block_pos*
////		     info->block.recbuffer));
////  DBUG_RETURN((byte*) info->block.level_info[0].last_blocks+
////	      block_pos*info->block.recbuffer);
////
//}

#endif // !defined(AFX_FLATSTORAGE_H__4EF56DD4_5674_444F_A9EC_01E0401C2E74__INCLUDED_)
