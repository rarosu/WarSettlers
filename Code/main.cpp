#include "WarSettlers.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	try
	{
		WarSettlers game(instance, showState);
		return game.Start();
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}