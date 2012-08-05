#include "WindowsInput.h" 

WindowsInput::WindowsInput()
{
}


WindowsInput::WindowsInput(const WindowsInput& other)
{
}


WindowsInput::~WindowsInput()
{
}



void WindowsInput::Init() 
{	
	for(int i=0; i<256; i++) 
	{
		m_keys[i] = false; 
	}
}


void WindowsInput::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void WindowsInput::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool WindowsInput::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}