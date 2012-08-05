#include <Windows.h>

#include "WindowsSystem.h"; 

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	bool windowsInitResult = false; 	
	WindowsSystem* system; 

	system = new WindowsSystem(); 
	windowsInitResult = system->Init(); 

	if(windowsInitResult) 
	{
		system->Run(); 
	}

	system->Shutdown(); 
	delete system; 

	return 0;
}