#include "GameTimer.hpp"
#include "Libs/r2tk/r2-exception.hpp"

namespace Framework 
{
	GameTimer::GameTimer(void)
	{
		m_lastTicks; 
		m_currentTicks; 
		m_frequency; 
		
		if(!QueryPerformanceFrequency(&m_frequency))
			throw r2ExceptionRuntimeM("Unable to use hardware timer (QueryPerformanceFrequency)");  
	}


	GameTimer::~GameTimer(void)
	{
	}

	/**
		Use the performance counter in windows to access the hardware high resolution timer
		calculate the time in seconds since last called. 
	*/
	double GameTimer::GetDt() 
	{				
		if(!QueryPerformanceCounter(&m_currentTicks)) 
			throw r2ExceptionRuntimeM("Unable to use hardware timer (QueryPerformanceCounter)");  

		if(m_currentTicks.QuadPart != m_lastTicks.QuadPart && m_frequency.QuadPart > 0) 
		{
			__int64 delta = 0; 
			delta = m_currentTicks.QuadPart - m_lastTicks.QuadPart; 
			m_dt = (double)delta / (double)m_frequency.QuadPart; 

			m_lastTicks = m_currentTicks; 
		}
		
		if(m_dt < 0) 
			m_dt = 0; 

		if(m_dt > 1.0) 
			m_dt = 1.0; 

		return(m_dt); 
	}
}