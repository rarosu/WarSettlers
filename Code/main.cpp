#include <Windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	MessageBox(NULL, "Compilation complete", "Success", MB_OK | MB_ICONINFORMATION);

	return 0;
}