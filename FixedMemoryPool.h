/*
 * @file	FixedMemoryPool.h
 * @author	LKF
 * @date	2014-11-5  20:29
 * @brief	定长内存池
 * @Copyright (C) 2014, by Bning
 */

#ifndef __FixedMemoryPool_H__
#define __FixedMemoryPool_H__
#include <_osdef.h>
#include <stdio.h>
#include <cstdlib>
//#include <Tick.h>
//#include <string.h>

//using namespace xlib::time::tick64;

class FixedMemoryPool
{
public:

#pragma pack(push, 1)
	typedef struct tagBuffer
	{
		size_t		dwSize;		//内存数据段大小
 		tagBuffer*	pPrevBuffer;//指向上一个内存块的指针
 		//TICKCOUNT	dwFreeTick;	//即将被销毁的时间

	}BUFFER, *PBUFFER;
#pragma pack(pop)


public:
	FixedMemoryPool(size_t length,size_t iniNum);
	~FixedMemoryPool(void);

	PVOID AllocMemory(const size_t& dwSize);
	VOID FreeMemory(const PVOID& ptr);
	VOID myprint(size_t iniNum);
	VOID CheckFreeMemory();
private:
	DISALLOW_COPY_AND_ASSIGN(FixedMemoryPool);

private:
	size_t m_bufferSize;
	int m_Num;
	int m_useNum;
	int m_errorNum;
	PBUFFER				m_pLastBuffer;		//指向最后一个空闲的内存块的指针
// 	size_t				m_dwFreedSize;		//已被释放的小型内存块大小
// 	size_t				m_dwAllocedSize;	//已被申请的小型内存块大小
};

typedef std::tr1::shared_ptr<FixedMemoryPool> FixedMemoryPoolPtr;

#endif // FixedMemoryPool_H__