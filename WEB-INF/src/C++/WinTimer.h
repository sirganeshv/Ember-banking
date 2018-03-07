#include "custom.hpp"
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

typedef void (*Timer_Callback)();

class CTimer
{
private:
	Timer_Callback m_callback;
	unsigned int m_msInterval;
	//DWORD m_msInterval;
	HANDLE m_hThread;
	bool m_bStarted;

	//Makes the thread sleep for the specific interval 
	static DWORD WINAPI InternalThread(LPVOID pParam) {
		CTimer * pTimer = (CTimer*)pParam;

		if(pTimer) {
				Sleep(pTimer->m_msInterval);
				pTimer->m_callback();
		}

		return 0;
	};
public:
	CTimer() {
		m_hThread = 0;
		m_callback = 0;
		m_msInterval = 2000;
		m_bStarted = false;
	}

	CTimer(Timer_Callback callback, unsigned int msInterval) {
		m_callback = callback;
		m_msInterval = msInterval;
		m_bStarted = false;
	}
	~CTimer() {
		Stop();
	}
	
	//Sets the interval 
	void SetInterval(unsigned int msInterval) {
		m_msInterval = msInterval;
	}
	
	//Creates a thread for timer
	void Start() {
		if(!m_bStarted)
			m_hThread = CreateThread(NULL, 0, InternalThread, (LPVOID)this, 0, 0);
		m_bStarted = true;
	}

	//Terminates the thread
	void Stop() {
		//cout<<"Gonna end";
		if(m_bStarted)
			TerminateThread(m_hThread, 0);
		m_bStarted = false;
	}
};

/*struct timeval {
	long tv_sec;
	long tv_usec;
};*/

/*void gettimeofday(struct timeval * tp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
}*/