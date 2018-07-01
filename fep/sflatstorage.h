 // SimpleFlatStorage.h: interface for the CSimpleFlatStorage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SIMPLE_FLAT_STAORAGE_H_
#define _SIMPLE_FLAT_STAORAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globaldef.h"

template<class TYPE>
class CSimpleFlatStorage  
{
public:
	CSimpleFlatStorage();
	virtual ~CSimpleFlatStorage();
	
protected:
	uint m_records_in_block;
	uint m_max_records;
	uint m_block_ptr_granularity;
	
	uint m_nBlockNr;
	TYPE** m_pBlocks;

	void GetNewBlock(const uint scale_to_block);
	
public:
	void Init(uint records_in_block = 128,
		uint max_records = 0x4000,
		uint block_ptr_granularity = 4);//call this first

	TYPE& operator[] (const uint pos);

	bool IsEmpty() { return m_pBlocks == 0; };
};

template<class TYPE>
CSimpleFlatStorage<TYPE>::CSimpleFlatStorage()
{
	m_records_in_block = 128;
	m_max_records = 0x4000;
	m_block_ptr_granularity = 4;

	m_nBlockNr = 0;
	m_pBlocks = 0;
}

template<class TYPE>
void CSimpleFlatStorage<TYPE>::Init(uint records_in_block,
						   uint max_records,
						   uint block_ptr_granularity)
{
	m_max_records = max_records;
	m_records_in_block = records_in_block;
	m_block_ptr_granularity = block_ptr_granularity;
}

template<class TYPE>
CSimpleFlatStorage<TYPE>::~CSimpleFlatStorage()
{
	for(uint n = 0; n < m_nBlockNr; n ++)
	{
		if(m_pBlocks[n])
			delete[] m_pBlocks[n];
	}

	free(m_pBlocks);
}

template<class TYPE>
TYPE& CSimpleFlatStorage<TYPE>::operator[] (const uint pos)
{
	TYPE* t = 0;

	if(pos < m_max_records)
	{
		uint block = pos / m_records_in_block;
		uint rec = pos % m_records_in_block;

		if(block >= m_nBlockNr)
		{
			GetNewBlock(block);
		}


		return m_pBlocks[block][rec];
	}

	return (TYPE&)*t;
}

template<class TYPE>
void CSimpleFlatStorage<TYPE>::GetNewBlock(const uint scale_to_block)
{
	while(scale_to_block >= m_nBlockNr)
	{
		if((m_nBlockNr % m_block_ptr_granularity) == 0)
		{
			//need to enlarge m_pBloks;
			m_pBlocks = (TYPE**)realloc(m_pBlocks, (m_nBlockNr + m_block_ptr_granularity) * sizeof(TYPE*));
		}

		m_pBlocks[m_nBlockNr] = new TYPE[m_records_in_block];

		m_nBlockNr ++;
	}
}

#endif /* _SIMPLE_FLAT_STAORAGE_H_ */
