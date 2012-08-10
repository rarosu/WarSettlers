#include "WarSettlers.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	try
	{
		return CreateAndStartGame(instance, showState);
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}