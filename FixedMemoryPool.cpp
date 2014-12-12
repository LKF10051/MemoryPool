#include "FixedMemoryPool.h"
#include <iostream>

FixedMemoryPool::FixedMemoryPool(size_t length, size_t iniNum)
	:m_bufferSize(length)
	,m_Num(0)// 暂时不用翻倍拓展
	,m_pLastBuffer(NULL)
	,m_useNum(0)
	,m_errorNum(0)
// 	,m_dwAllocedSize(0)
// 	,m_dwFreedSize(0)
{
	
}


FixedMemoryPool::~FixedMemoryPool(void)
{
	PBUFFER pBuffer, pPrevBuffer;
	pBuffer = m_pLastBuffer;
	while ( pBuffer )
	{
		pPrevBuffer = pBuffer->pPrevBuffer;
		std::free( pBuffer );
		pBuffer = pPrevBuffer;
	}
}

PVOID FixedMemoryPool::AllocMemory( const size_t& dwSize )
{
	if ( dwSize > m_bufferSize ){
		return NULL;
	}

	PBUFFER pBuffer, pPrev, pResult = NULL;
	if ( m_pLastBuffer )
	{
		pResult = m_pLastBuffer;
		m_pLastBuffer = pResult->pPrevBuffer;
		pResult->pPrevBuffer = NULL;
		//m_dwFreedSize -= m_bufferSize;
	}
	else 
	{
		++m_Num;
		pResult = (PBUFFER)std::malloc( m_bufferSize + sizeof(*pResult) );
		pResult->dwSize = m_bufferSize;
		pResult->pPrevBuffer = NULL;
	}
	//m_dwAllocedSize += m_bufferSize;
	//++m_Num;
	++m_useNum;
	return pResult ? pResult + 1 : NULL;
}

VOID FixedMemoryPool::FreeMemory( const PVOID& ptr )
{
	if (!ptr){ 
		return;
	}

	PBUFFER pBuffer = ((PBUFFER)ptr) - 1;
	if ( pBuffer->dwSize == m_bufferSize )
	{
		pBuffer->pPrevBuffer = m_pLastBuffer;
		m_pLastBuffer = pBuffer;
// 		m_dwAllocedSize -= m_bufferSize;
// 		m_dwFreedSize += m_bufferSize;
		//--m_Num;
		--m_useNum;
	}
	else
	{
		++m_errorNum;
	}
}

void FixedMemoryPool::myprint(size_t iniNum)
{
	//printf("size = %d       TotolNum = %d     use = %d     error = %d   \n",m_bufferSize,m_Num, m_useNum,m_errorNum);
	int freeM = ((m_Num - m_useNum)*m_bufferSize)/(1024*1024);
	int useM = ((m_useNum)*m_bufferSize)/(1024*1024);
	printf("size = %8d TotolNum = %8d use = %8d freeM = %8d useM = %8d error = %8d\n"
		,m_bufferSize,  m_Num,        m_useNum , freeM,     useM ,      m_errorNum);
}

VOID FixedMemoryPool::CheckFreeMemory()
{
	int freeNum = m_Num - (m_useNum +1)*2;
	int freeMemory = ((m_Num - m_useNum)*m_bufferSize)/(1024*1024);
	if (freeNum <= 0 || freeMemory < 20){
		return;
	}

	PBUFFER pBuffer, pPrevBuffer;
	pBuffer = m_pLastBuffer;
	for (int i = 0; i < freeNum; i++)
	{
		pPrevBuffer = pBuffer->pPrevBuffer;
		std::free( pBuffer );
		pBuffer = pPrevBuffer;
	}
	m_pLastBuffer = pBuffer;
	m_Num -= freeNum;
}
