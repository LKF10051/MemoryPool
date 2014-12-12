#include "MemoryChunk.h"
#include "MemoryPool.h"

MemoryChunk::MemoryChunk(const size_t& size)
	:m_pointer(NULL)
	,m_offset(0)
	,m_size(size)
{
	//printf("======MemoryChunk========\n");
	m_pointer = MemoryPool::Instance()->AllocMemory(size);
}


MemoryChunk::~MemoryChunk(void)
{
	//printf("======~~~~~~MemoryChunk========\n");
	if (m_pointer != NULL){
		MemoryPool::Instance()->FreeMemory(m_pointer,m_size);
	}
}
