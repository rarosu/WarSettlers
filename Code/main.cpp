#include <Framework\Window.hpp>
#include <Framework\D3DContext.hpp>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	/*
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
	*/

	// Create a window
	Framework::Window::Description windowDesc;
	windowDesc.m_caption = "WarSettlers";
	windowDesc.m_clientHeight = 480;
	windowDesc.m_clientWidth = 640;
	windowDesc.m_hasFrame = true;
	windowDesc.m_resizable = false;
	windowDesc.m_showState = showState;
	windowDesc.m_x = 0;
	windowDesc.m_y = 0;

	Framework::Window window(instance, windowDesc);

	// Create a D3D11 context
	Framework::D3DContext::Description contextDesc;
	contextDesc.m_viewports.push_back(Framework::D3DContext::Viewport());

	Framework::D3DContext context(&window, contextDesc);


	// Run the main loop
	bool running = true;
	while (running)
	{
		running = window.ProcessMessages();

		context.Clear(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		context.SwapBuffers();
	}

	return window.GetExitValue();
}