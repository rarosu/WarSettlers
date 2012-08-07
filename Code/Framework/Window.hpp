#ifndef FRAMEWORK_WINDOW_HPP
#define FRAMEWORK_WINDOW_HPP

#include <string>

#include "WinInclude.hpp"
#include "InputManager.hpp"
#include <Libs\r2tk\r2-singleton.hpp>

namespace Framework
{
	/**
		Interface for listening to window events, excluding input.
	*/
	class WindowEventListener
	{
	public:
		virtual void WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight) {}
	};

	/**
		Manages a basic window and its events. Designed to work
		as a rendering surface.
	*/
	class Window : public r2::Singleton<Window>
	{
	public:
		/**
			Describes the properties of a window, for creation.
		*/
		struct Description
		{
			// Determines how the window is shown when created (minimized, hidden, shown, etc.).
			int m_showState;

			// The position of the window. Use negative value for default.
			int m_x;
			int m_y;

			// The dimensions of the client area of the window.
			unsigned int m_clientWidth;
			unsigned int m_clientHeight;

			// The title caption of the window.
			std::string m_caption;

			// Whether the window can be resized.
			bool m_resizable;

			// Whether the window has a frame.
			bool m_hasFrame;


			Description();
		};


		Window(HINSTANCE instance, const Description& description);
		

		unsigned int GetWindowWidth() const;
		unsigned int GetWindowHeight() const;
		unsigned int GetClientWidth() const;
		unsigned int GetClientHeight() const;
		HWND GetHandle() const;
		HINSTANCE GetInstance() const;


		// Process all messages in the message queue.
		bool ProcessMessages();

		// Get the value the application exited with. Returns 0 on a good day, or
		// if the window hasn't been destroyed yet.
		int GetExitValue();

		// Add/Remove window event listeners
		void AddEventListener(WindowEventListener* listener);
		void RemoveEventListener(WindowEventListener* listener);
		bool IsEventListener(WindowEventListener* listener) const;
	private:
		RECT m_clientRect;
		RECT m_windowRect;
		int m_exitValue;
		HINSTANCE m_instance;
		HWND m_handle;

		InputManager m_inputManager;
		std::vector<WindowEventListener*> m_eventListeners;

		
		// Static WndProc. Forwards every message to the window class.
		static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

		// Methods for creating a window from a description.
		bool SetupClass(const Description& description);
		bool SetupWindow(const Description& description);

		// Methods for handling messages from the WndProc.
		bool HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam);
		void HandleSizeMessage();
	};
}

#endif