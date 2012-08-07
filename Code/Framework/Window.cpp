#include "Window.hpp"

#include <algorithm>
#include <Libs\r2tk\r2-exception.hpp>

namespace Framework
{
	Window::Description::Description()
		: m_showState(SW_SHOW)
		, m_x(-1)
		, m_y(-1)
		, m_clientWidth(640)
		, m_clientHeight(480)
		, m_caption("Unnamed Application")
		, m_resizable(true)
		, m_hasFrame(true)
	{}

	Window::Window(HINSTANCE instance, const Description& description)
		: m_instance(instance)
		, m_handle(NULL)
		, m_exitValue(0)
	{
		if (!SetupClass(description))
			throw r2ExceptionRuntimeM("Failed to setup window class");
		if (!SetupWindow(description))
			throw r2ExceptionRuntimeM("Failed to create window");
	}


	unsigned int Window::GetWindowWidth() const
	{
		return GetRectWidth(m_windowRect);
	}

	unsigned int Window::GetWindowHeight() const
	{
		return GetRectHeight(m_windowRect);
	}

	unsigned int Window::GetClientWidth() const
	{
		return GetRectWidth(m_clientRect);
	}

	unsigned int Window::GetClientHeight() const
	{
		return GetRectHeight(m_clientRect);
	}

	HWND Window::GetHandle() const
	{
		return m_handle;
	}

	HINSTANCE Window::GetInstance() const
	{
		return m_instance;
	}


	bool Window::ProcessMessages()
	{
		m_inputManager.NewFrame();

		MSG message;
		ZeroMemory(&message, sizeof(message));

		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT)
			m_exitValue = (int) message.wParam;

		return message.message != WM_QUIT;
	}

	int Window::GetExitValue()
	{
		return m_exitValue;
	}

	void Window::AddEventListener(WindowEventListener* listener)
	{
		// Add the listener, if it isn't already in the list.
		if (std::find(m_eventListeners.begin(), m_eventListeners.end(), listener) == m_eventListeners.end())
			m_eventListeners.push_back(listener);
	}

	void Window::RemoveEventListener(WindowEventListener* listener)
	{
		// Remove the listener, if we find it in the list.
		std::vector<WindowEventListener*>::iterator it = std::find(m_eventListeners.begin(), m_eventListeners.end(), listener);
		if (it != m_eventListeners.end())
			m_eventListeners.erase(it);
	}

	bool Window::IsEventListener(WindowEventListener* listener) const
	{
		return std::find(m_eventListeners.begin(), m_eventListeners.end(), listener) != m_eventListeners.end();
	}




	LRESULT CALLBACK Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (InstancePtr() != NULL && Instance().m_handle != NULL)
			if (Instance().HandleWindowMessage(message, wParam, lParam))
				return 0;
		return DefWindowProc(handle, message, wParam, lParam);
	}


	bool Window::SetupClass(const Description& description)
	{
		WNDCLASSEX windowClass;

		windowClass.cbSize			= sizeof(WNDCLASSEX);
		windowClass.style			= CS_VREDRAW | CS_HREDRAW;
		windowClass.cbClsExtra		= 0;
		windowClass.cbWndExtra		= 0;
		windowClass.hInstance		= m_instance;
		windowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWTEXT + 1);
		windowClass.hCursor			= LoadCursor(0, IDC_ARROW);
		windowClass.hIcon			= LoadIcon(0, IDI_APPLICATION);
		windowClass.hIconSm			= 0;
		windowClass.lpfnWndProc		= WndProc;
		windowClass.lpszClassName	= "ApplicationWindow";
		windowClass.lpszMenuName	= 0;

		if (!RegisterClassEx(&windowClass))
			return false;

		return true;
	}

	bool Window::SetupWindow(const Description& description)
	{
		DWORD style = WS_OVERLAPPEDWINDOW;
		unsigned int windowX = (description.m_x < 0) ? CW_USEDEFAULT : description.m_x;
		unsigned int windowY = (description.m_y < 0) ? CW_USEDEFAULT : description.m_y;

		if (!description.m_resizable)
		{
			style &= ~WS_THICKFRAME;
			style &= ~WS_MAXIMIZEBOX;
		}

		if (!description.m_hasFrame)
		{
			style &= ~WS_SYSMENU;
			style &= ~WS_OVERLAPPED;
			style &= ~WS_CAPTION;
			style &= ~WS_THICKFRAME;
			style |= WS_POPUP;
		}
		
		m_clientRect.left = 0;
		m_clientRect.right = description.m_clientWidth;
		m_clientRect.top = 0;
		m_clientRect.bottom = description.m_clientHeight;

		m_windowRect = m_clientRect;
		AdjustWindowRect(&m_windowRect, style, FALSE);

		m_handle = CreateWindow("ApplicationWindow",
			description.m_caption.c_str(),
			style,
			windowX,
			windowY,
			GetRectWidth(m_windowRect),
			GetRectHeight(m_windowRect),
			NULL,
			NULL,
			m_instance,
			NULL);

		if (m_handle == NULL)
			return false;

		ShowWindow(m_handle, description.m_showState);
		UpdateWindow(m_handle);

		return true;
	}

	bool Window::HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
			case WM_PAINT:
				hdc = BeginPaint(m_handle, &ps);
				EndPaint(m_handle, &ps);
				return true;
			case WM_DESTROY:
				PostQuitMessage(0);
				return true;
			case WM_SIZE:
				HandleSizeMessage();
				return true;

			case WM_KEYDOWN:
				m_inputManager.HandleKeyDownMessage(wParam, lParam);
				return true;
			case WM_KEYUP:
				m_inputManager.HandleKeyUpMessage(wParam, lParam);
				return true;
			case WM_CHAR:
				m_inputManager.HandleCharMessage(wParam, lParam);
				return true;

			case WM_LBUTTONDOWN:
				m_inputManager.HandleButtonDownMessage(Button::Left, wParam);
				return true;
			case WM_RBUTTONDOWN:
				m_inputManager.HandleButtonDownMessage(Button::Right, wParam);
				return true;
			case WM_MBUTTONDOWN:
				m_inputManager.HandleButtonDownMessage(Button::Middle, wParam);
				return true;
			case WM_LBUTTONUP:
				m_inputManager.HandleButtonUpMessage(Button::Left, wParam);
				return true;
			case WM_RBUTTONUP:
				m_inputManager.HandleButtonUpMessage(Button::Right, wParam);
				return true;
			case WM_MBUTTONUP:
				m_inputManager.HandleButtonUpMessage(Button::Middle, wParam);
				return true;

			case WM_MOUSEMOVE:
				m_inputManager.HandleMouseMoveMessage(wParam, lParam);
				return true;
		}

		return false;
	}

	void Window::HandleSizeMessage()
	{
		WINDOWINFO info;
		GetWindowInfo(m_handle, &info);

		bool resized = false;
		if (GetRectWidth(info.rcClient) != GetRectWidth(m_clientRect))
			resized = true;
		else if (GetRectHeight(info.rcClient) != GetRectHeight(m_clientRect))
			resized = true;

		m_clientRect = info.rcClient;
		m_windowRect = info.rcWindow;
				
		if (resized)
		{
			unsigned int clientWidth = GetClientWidth();
			unsigned int clientHeight = GetClientHeight();
			unsigned int windowWidth = GetWindowWidth();
			unsigned int windowHeight = GetWindowHeight();

			for (size_t i = 0; i < m_eventListeners.size(); ++i)
			{
				m_eventListeners[i]->WindowResized(clientWidth, clientHeight, windowWidth, windowHeight);
			}
		}
	}
}