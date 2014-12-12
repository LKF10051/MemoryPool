#include "MemoryPool.h"
#include <math.h>
MemoryPool* MemoryPool::m_instance = NULL;
CommonMutex MemoryPool::m_instanceMutex;
CommonMutex MemoryPool::m_mapMutex;
MemoryPool::MemoryPool( void )
	:m_isRunning(true)
{
	InitializePool();
	m_mycount = 5000;
// 	m_pTimeThread = new xlib::thread::CBaseThread("MemoryPoolTime", staticConnectThreadRountine, this);
// 	m_pTimeThread->StartWorkThread();
}

MemoryPool::~MemoryPool( void )
{
	m_isRunning = false;
//	m_pTimeThread->StopWorkThread();
}

void MemoryPool::InitializePool()
{
	typedef map<size_t,FixedMemoryPoolPtr>::value_type MapType;
	int baseLength = 64;
	for (int i = 0; i <13; i++ )
	{
		size_t poolLength = baseLength * pow(2.0,i);
		FixedMemoryPoolPtr fxd = FixedMemoryPoolPtr(new FixedMemoryPool(poolLength, 32));
		m_FixedMemoryPoolMap.insert(MapType(poolLength,fxd));
	}
}

MemoryPool* MemoryPool::Instance()
{
	CommonMutex::CommonLock locker(m_instanceMutex);
	if( NULL == m_instance){
		m_instance = new MemoryPool;
	}
	return m_instance;
}

PVOID MemoryPool::AllocMemory( size_t dwSize )
{
	PVOID ret;
	typedef map<size_t,FixedMemoryPoolPtr>::value_type MapType;
	CommonMutex::CommonLock locker(m_mapMutex);


	//--m_mycount;
// 	if (m_mycount%100000 == 0)
// 	{
// 		printf("=========m_mycount====%d=====\n",m_mycount);
// 	}
// 	if (m_mycount == 0)
// 	{
// 		printf("=========m_mycount====%d=====\n",m_mycount);
// 		m_mycount = 5000;
// 		poolprint(0);
// 	}


	size_t suitableLength = GetSuitableLength(dwSize);
	if (m_FixedMemoryPoolMap.find(suitableLength) == m_FixedMemoryPoolMap.end())// 没有适合长度的池,需要拓展
	{
		FixedMemoryPoolPtr fxd = FixedMemoryPoolPtr(new FixedMemoryPool(suitableLength, 1)); // 太大的池，初始化1个
		m_FixedMemoryPoolMap.insert(MapType(suitableLength,fxd));
	}

	ret = m_FixedMemoryPoolMap[suitableLength]->AllocMemory(suitableLength);
	return ret;
}

VOID MemoryPool::FreeMemory( PVOID memory ,size_t dwSize )
{
	CommonMutex::CommonLock locker(m_mapMutex);
	size_t suitableLength = GetSuitableLength(dwSize);
	if (m_FixedMemoryPoolMap.find(suitableLength) == m_FixedMemoryPoolMap.end()){
		return;
	}

	m_FixedMemoryPoolMap[suitableLength]->FreeMemory(memory);
}

size_t MemoryPool::GetSuitableLength( const size_t& dwSize )
{
	size_t ret = 0;
	int baseLength = 64;
	for (int i = 0; i <13; i++ )
	{
		size_t poolLength = baseLength * pow(2.0,i);
		if (dwSize <= poolLength)
		{
			ret = poolLength;
			break;
		}
	}

	if (ret == 0)// 没有适合长度的池
	{
		for (int i = 13; i<22; i++ ) 
		{
			size_t poolLength = baseLength * pow(2.0,i);
			if (dwSize <= poolLength)
			{
				ret = poolLength;
				break;
			}
		}
	}
	return ret;
}

void MemoryPool::poolprint(size_t iniNum)
{
	typedef map<size_t,FixedMemoryPoolPtr>::iterator MapItr;
	typedef map<size_t,FixedMemoryPoolPtr>::value_type MapType;

	PVOID ret;

	MapItr it = m_FixedMemoryPoolMap.begin();
	for (;it != m_FixedMemoryPoolMap.end(); it++){
		it->second->myprint(0);
	}
}

void MemoryPool::staticConnectThreadRountine( LPVOID pParam )
{
	int sleepTime = 30000;
	Sleep(sleepTime);
	while(MemoryPool::Instance()->CheckFreeMemory()) // 循环检测
	{
		Sleep(sleepTime);
	}
}

bool MemoryPool::CheckFreeMemory()
{
	typedef map<size_t,FixedMemoryPoolPtr>::iterator MapItr;
	typedef map<size_t,FixedMemoryPoolPtr>::value_type MapType;
	CommonMutex::CommonLock locker(m_mapMutex);
	MapItr it = m_FixedMemoryPoolMap.begin();
	for (;it != m_FixedMemoryPoolMap.end(); it++)
	{
// 		size_t nlength = it->first;
// 		FixedMemoryPoolPtr fxd = it->second;
// 		if (nlength > 262144){//256K
// 			fxd->CheckFreeMemory();
// 		}
		it->second->CheckFreeMemory();
	}
	poolprint(0);
	return m_isRunning;
}
