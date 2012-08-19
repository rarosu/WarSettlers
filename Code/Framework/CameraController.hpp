#ifndef FRAMEWORK_CAMERACONTROLLER_HPP
#define FRAMEWORK_CAMERACONTROLLER_HPP

#include "Camera.hpp" 
#include "Window.hpp"

namespace Framework
{

	class CameraController : public Framework::WindowEventListener
	{
	public:
		CameraController(); 
		CameraController(Camera *camera);
		virtual ~CameraController(); 

		void WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight); 
		virtual void Update(double dt) = 0; 

	protected: 
		Camera *m_camera; 
	};

}
#endif