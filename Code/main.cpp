#include <Utility\Logger.hpp>
#include <Libs\Effects11\d3dx11effect.h>
#include "WarSettlers.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	Utility::Logger logger("WarSettlers.log");

	try
	{
		return CreateAndStartGame(instance, showState);
	}
	catch (std::exception& e)
	{
		Utility::Logger::Instance().E(e.what());
	}

	return 0;
}