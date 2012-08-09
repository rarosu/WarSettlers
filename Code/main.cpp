#include <vector>
#include <algorithm>

#include <Framework\Window.hpp>
#include <Framework\DisplayCapabilities.hpp>
#include <Framework\D3DContext.hpp>

struct Resolution
{
	Resolution(int size)
		: m_width(size)
		, m_height(size)
	{}

	Resolution(int width, int height)
		: m_width(width)
		, m_height(height)
	{}

	bool operator>(const Resolution& rhs) const
	{
		return m_width > rhs.m_width;
	}

	int m_width;
	int m_height;
};

typedef Resolution SortElement;

bool SortPredicate(const SortElement& lhs, const SortElement& rhs)
{
	return lhs > rhs;
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showState)
{
	/*
	std::vector<SortElement> test;
	test.push_back(SortElement(5));
	test.push_back(SortElement(7));
	test.push_back(SortElement(-1));
	test.push_back(SortElement(11));
	test.push_back(SortElement(3));
	test.push_back(SortElement(2));

	std::sort(test.begin(), test.end(), SortPredicate);

	return 0;
	*/

	// Query DXGI for supported display modes
	unsigned int currentDisplayMode = 0;
	Framework::DisplayCapabilities displayCapabilities(false);


	// Create a window
	Framework::Window::Description windowDesc;
	windowDesc.m_caption = "WarSettlers";
	windowDesc.m_clientWidth = displayCapabilities.m_displayModes[0].m_description.Width;
	windowDesc.m_clientHeight = displayCapabilities.m_displayModes[0].m_description.Height;
	windowDesc.m_hasFrame = true;
	windowDesc.m_resizable = false;
	windowDesc.m_showState = showState;
	windowDesc.m_x = 0;
	windowDesc.m_y = 0;

	Framework::Window window(instance, windowDesc);


	// Create a D3D11 context
	Framework::D3DContext::Description contextDesc;
	contextDesc.m_displayMode = displayCapabilities.m_displayModes[currentDisplayMode];
	contextDesc.m_fullscreen = false;
	contextDesc.m_vsync = true;
	contextDesc.m_viewports.push_back(Framework::D3DContext::Viewport());

	Framework::D3DContext context(&window, contextDesc);


	// Run the main loop
	bool running = true;
	while (running)
	{
		running = window.ProcessMessages();

		const Framework::InputState& currentInput = Framework::InputManager::Instance().GetCurrentInput();
		const Framework::InputState& previousInput = Framework::InputManager::Instance().GetPreviousInput();

		// switch resolution on R key pressed
		if (currentInput.m_keyboard.m_keys['R'] && !previousInput.m_keyboard.m_keys['R'])
		{
			++currentDisplayMode;
			currentDisplayMode %= displayCapabilities.m_displayModes.size();

			context.ChangeResolution(displayCapabilities.m_displayModes[currentDisplayMode], context.IsFullscreen(), context.IsVSyncEnabled());
		}

		// switch to/from fullscreen on F key pressed
		if (currentInput.m_keyboard.m_keys['F'] && !previousInput.m_keyboard.m_keys['F'])
		{
			context.ToggleFullscreen();
		}

		context.Clear(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		context.SwapBuffers();
	}

	return window.GetExitValue();
}