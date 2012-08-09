#include "InputManager.hpp"

#include <algorithm>

namespace Framework
{
	InputState::Keyboard::Keyboard()
	{
		ZeroMemory(m_keys, sizeof(bool) * 256);
	}

	InputState::Mouse::Mouse()
		: m_x(0)
		, m_y(0)
	{
		ZeroMemory(m_buttons, sizeof(bool) * Button::Count);
	}



	const InputState& InputManager::GetCurrentInput() const
	{
		return m_currentInput;
	}

	const InputState& InputManager::GetPreviousInput() const
	{
		return m_previousInput;
	}



	void InputManager::AddInputListener(InputListener* listener)
	{
		// Add the listener, if it isn't already in the list.
		if (std::find(m_inputListeners.begin(), m_inputListeners.end(), listener) == m_inputListeners.end())
			m_inputListeners.push_back(listener);
	}

	void InputManager::RemoveInputListener(InputListener* listener)
	{
		// Remove the listener, if we find it in the list.
		std::vector<InputListener*>::iterator it = std::find(m_inputListeners.begin(), m_inputListeners.end(), listener);
		if (it != m_inputListeners.end())
			m_inputListeners.erase(it);
	}

	bool InputManager::IsListeningForInput(InputListener* listener) const
	{
		return std::find(m_inputListeners.begin(), m_inputListeners.end(), listener) != m_inputListeners.end();
	}



	void InputManager::NewFrame()
	{
		m_previousInput = m_currentInput;
	}

	void InputManager::HandleKeyDownMessage(WPARAM wParam, LPARAM lParam)
	{
		m_currentInput.m_keyboard.m_keys[wParam] = true;
		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->KeyPressed(static_cast<int>(wParam));
		}
	}

	void InputManager::HandleKeyUpMessage(WPARAM wParam, LPARAM lParam)
	{
		m_currentInput.m_keyboard.m_keys[wParam] = false;
		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->KeyReleased(static_cast<int>(wParam));
		}
	}

	void InputManager::HandleCharMessage(WPARAM wParam, LPARAM lParam)
	{
		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->CharEntered(static_cast<char>(wParam));
		}
	}

	void InputManager::HandleButtonDownMessage(Button::Button button, LPARAM lParam)
	{
		m_currentInput.m_mouse.m_buttons[button] = true;
		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->ButtonPressed(button, LOWORD(lParam), HIWORD(lParam));
		}
	}

	void InputManager::HandleButtonUpMessage(Button::Button button, LPARAM lParam)
	{
		m_currentInput.m_mouse.m_buttons[button] = false;
		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->ButtonReleased(button, LOWORD(lParam), HIWORD(lParam));
		}
	}

	void InputManager::HandleMouseMoveMessage(WPARAM wParam, LPARAM lParam)
	{
		m_currentInput.m_mouse.m_x = LOWORD(lParam);
		m_currentInput.m_mouse.m_y = HIWORD(lParam);

		for (size_t i = 0; i < m_inputListeners.size(); ++i)
		{
			m_inputListeners[i]->MouseMoved(m_currentInput.m_mouse.m_x, 
				m_currentInput.m_mouse.m_y,
				static_cast<int>(m_currentInput.m_mouse.m_x) - static_cast<int>(m_previousInput.m_mouse.m_x),
				static_cast<int>(m_currentInput.m_mouse.m_y) - static_cast<int>(m_previousInput.m_mouse.m_y));
		}
	}


}