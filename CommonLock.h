/*
 * @file	CommonLock.h
 * @author	LKF
 * @date	2014-11-5  11:47
 * @brief	ËøÀà
 * @Copyright (C) 2014, by Bning
 */
#ifndef __CommonLock_H__
#define __CommonLock_H__

#include "_osdef.h"

class CommonMutex {
public:

	CommonMutex()  { m_lGrdCnt = 0; InitializeCriticalSection(&m_cs); }
	~CommonMutex() { DeleteCriticalSection(&m_cs); }

	BOOL IsLocked() const { return(m_lGrdCnt > 0); }

public:
	class CommonLock {
	public:
		CommonLock(CommonMutex& rg) : m_rg(rg) { m_rg.LockUp(); };
		~CommonLock() { m_rg.LockOff(); }

	private:
		CommonMutex& m_rg;
	};

private:
	void LockUp()   { EnterCriticalSection(&m_cs); m_lGrdCnt++; }
	void LockOff() { m_lGrdCnt--; LeaveCriticalSection(&m_cs); }

	
	friend class CommonMutex::CommonLock;

private:
	CRITICAL_SECTION m_cs;
	long m_lGrdCnt;   // # of EnterCriticalSection calls
};

#endif // CommonLock_H__