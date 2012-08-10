#include <Utility\Logger.hpp>
#include "WarSettlers.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	Utility::Logger logger("WarSettlers.log");

	try
	{
		WarSettlers game(instance, showState);
		return game.Start();
	}
	catch (std::exception& e)
	{
		Utility::Logger::Instance().E(e.what());
	}

	return 0;
}