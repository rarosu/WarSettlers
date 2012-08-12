#include "GameTimer.hpp"

namespace Framework 
{
	GameTimer::GameTimer(void)
	{
		m_lastTicks; 
		m_currentTicks; 
		m_frequency; 

		m_zeroTime = 0.001; 
		m_maxTime = 1.0; 

		QueryPerformanceFrequency(&m_frequency); 
	}

	GameTimer::GameTimer(double maxTime, double zeroTime)
	{
		m_lastTicks; 
		m_currentTicks; 
		m_frequency; 

		m_maxTime = maxTime; 
		m_zeroTime = zeroTime; 

		QueryPerformanceFrequency(&m_frequency); 
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
		QueryPerformanceCounter(&m_currentTicks); 

		if(m_currentTicks.QuadPart != m_lastTicks.QuadPart && m_frequency.QuadPart > 0) 
		{
			__int64 delta = 0; 
			delta = m_currentTicks.QuadPart - m_lastTicks.QuadPart; 
			m_dt = (double)delta / (double)m_frequency.QuadPart; 

			m_lastTicks = m_currentTicks; 
		}

		// adjust for extreme values 
		if(m_dt > m_maxTime) 
			m_dt = m_maxTime; 

		if(m_dt == 0) 
			m_dt = m_zeroTime; 


		return(m_dt); 
	}
}