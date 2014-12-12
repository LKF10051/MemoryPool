/*
 * @file	MemoryPool.h
 * @author	LKF
 * @date	2014-11-5  11:44
 * @brief	内存池，替换 CBufferAllocator
 * @Copyright (C) 2014, by Bning
 */
#ifndef __MemoryPool_H__
#define __MemoryPool_H__
#include <map>
#include "MemoryChunk.h"
#include "CommonLock.h"
#include "FixedMemoryPool.h"
#include "Thread.h"

using namespace std;

class MemoryPool
{
public:
	static MemoryPool* Instance();

	PVOID AllocMemory(size_t dwSize);
	VOID FreeMemory(PVOID memory ,size_t dwSize);
	VOID poolprint(size_t iniNum);
private:
	DISALLOW_COPY_AND_ASSIGN(MemoryPool);
	MemoryPool(void);
	~MemoryPool(void);
	void InitializePool();
	size_t GetSuitableLength(const size_t& dwSize);
	static void staticConnectThreadRountine(LPVOID pParam);
	bool CheckFreeMemory();

private:
	static MemoryPool* m_instance;
	static CommonMutex m_instanceMutex;
	static CommonMutex m_mapMutex;
	//CommonMutex m_mapMutex;
	map<size_t,FixedMemoryPoolPtr> m_FixedMemoryPoolMap;

	int m_mycount;
	xlib::thread::CBaseThread				*m_pTimeThread;		// 计时器线程

	bool m_isRunning;
};

#endif // MemoryPool_H__