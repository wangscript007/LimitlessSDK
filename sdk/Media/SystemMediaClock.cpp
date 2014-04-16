#include "Media/SystemMediaClock.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>      

SystemMediaClock::SystemMediaClock()
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime;

	QueryPerformanceFrequency(&frequency);
	m_frequency=frequency.QuadPart;

	QueryPerformanceCounter(&startTime);
//	m_startTime=startTime.QuadPart;
	m_startTime=0;

	m_resolution=NSECS_PER_SECOND/m_frequency;
}

MediaTime SystemMediaClock::internalTime()
{
	LARGE_INTEGER counterValue;
	
	QueryPerformanceCounter(&counterValue);

	return (counterValue.QuadPart-m_startTime)*m_resolution;
}

MediaTime SystemMediaClock::resolution()
{
	return 1;
}