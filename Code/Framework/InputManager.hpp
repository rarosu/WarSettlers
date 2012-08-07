#ifndef FRAMEWORK_INPUT_MANAGER_HPP
#define FRAMEWORK_INPUT_MANAGER_HPP

#include <vector>

#include "WinInclude.hpp"
#include <Libs\r2tk\r2-singleton.hpp>

namespace Framework
{
	/**
		Forward declare
	*/
	class Window;

	/**
		Enumerating the mouse buttons
	*/
	namespace Button
	{
		enum Button { Left, Right, Middle, Count };
	}


	/**
		The state of the user input during one frame
	*/
	struct InputState
	{
		struct Keyboard
		{
			bool m_keys[256];

			Keyboard();
		} Keyboard;

		struct Mouse
		{
			unsigned int m_x;
			unsigned int m_y;
			bool m_buttons[Button::Count];

			Mouse();
		} Mouse;
	};


	/**
		Interface for listening to input event
	*/
	class InputListener
	{
	public:
		virtual void KeyPressed(int keyCode) {}
		virtual void KeyReleased(int keyCode) {}
		virtual void CharEntered(char character) {}
		virtual void MouseMoved(unsigned int x, unsigned int y, int dx, int dy) {}
		virtual void ButtonPressed(Framework::Button::Button button) {}
		virtual void ButtonReleased(Framework::Button::Button button) {}
	};


	/**
		Manages updating input, holding the input states and
		notifying game objects of events.
	*/
	class InputManager : public r2::Singleton<InputManager>
	{
	public:
		friend class Window;

		const InputState& GetCurrentInput() const;
		const InputState& GetPreviousInput() const;

		void AddInputListener(InputListener* listener);
		void RemoveInputListener(InputListener* listener);
		bool IsListeningForInput(InputListener* listener) const;
	private:
		InputState m_currentInput;
		InputState m_previousInput;

		std::vector<InputListener*> m_inputListeners;


		void NewFrame();
		void HandleKeyDownMessage(WPARAM wParam, LPARAM lParam);
		void HandleKeyUpMessage(WPARAM wParam, LPARAM lParam);
		void HandleCharMessage(WPARAM wParam, LPARAM lParam);
		void HandleButtonDownMessage(Button::Button button, LPARAM lParam);
		void HandleButtonUpMessage(Button::Button button, LPARAM lParam);
		void HandleMouseMoveMessage(WPARAM wParam, LPARAM lParam);
	};
}

#endif