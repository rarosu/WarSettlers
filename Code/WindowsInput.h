#ifndef _WINDOWSINPUT_H_
#define _WINDOWSINPUT_H_

class WindowsInput 
{
	public:
	WindowsInput();
	WindowsInput(const WindowsInput&);
	~WindowsInput();

	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
}; 

#endif 