#ifndef FRAMEWORK_GAMETIMER_HPP
#define FRAMEWORK_GAMETIMER_HPP

#include <windows.h>

namespace Framework 
{
	/**
		Manages the high resolution hardware timer. 
	*/ 
	class GameTimer
	{
	public:
		GameTimer(void);
		GameTimer(double maxTime, double zeroTime); 
		~GameTimer(void);

		// Gets the deltatime in fractions of seconds since last time this method was called. 
		double GetDt(); 
	private: 
		LARGE_INTEGER m_currentTicks;
		LARGE_INTEGER m_lastTicks; 
		LARGE_INTEGER m_frequency; 	
		double m_dt; 
	};

}
#endif