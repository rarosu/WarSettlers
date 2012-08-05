#ifndef _WINDOWSSYSTEM_H_
#define _WINDOWSSYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "WindowsInput.h" 

class WindowsSystem 
{
public: 
	WindowsSystem(); 
	WindowsSystem(const WindowsSystem&); 
	~WindowsSystem(); 

	bool Init(); 
	void Run(); 
	bool Frame(); 
	void Shutdown(); 

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private: 
	void InitWindows(); 

	WindowsInput *m_Input; 

	LPCSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int m_screenWidth; 
	int m_screenHeight; 
	bool m_fullScreen; 
}; 


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static WindowsSystem* ApplicationHandle = 0;

#endif