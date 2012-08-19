#ifndef FRAMEWORK_FPS_CAMERACONTROLLER_HPP
#define FRAMEWORK_FPS_CAMERACONTROLLER_HPP

#include "Window.hpp"
#include "InputManager.hpp"
#include "CameraController.hpp"
#include "Camera.hpp"

namespace Framework
{
	class FPSCameraController : public CameraController, Framework::InputListener
	{
	public:
		FPSCameraController(); 
		FPSCameraController(Camera *camera); 
		~FPSCameraController(); 
		void KeyPressed(int keyCode);
		void KeyReleased(int keyCode);
		void MouseMoved(unsigned int x, unsigned int y, int dx, int dy); 
		
		void Update(double dt); 
		const D3DXMATRIX& GetViewProjection() const;
	private:
		static const double C_VELOCITY;
		
		int m_movingForward; 
		int m_movingSideways; 

	};
}

#endif