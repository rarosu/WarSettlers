#ifndef FRAMEWORK_FPS_CAMERACONTROLLER_HPP
#define FRAMEWORK_FPS_CAMERACONTROLLER_HPP

#define _USE_MATH_DEFINES

#include <math.h>
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
		FPSCameraController(Camera *camera, int screenWidth, int screenHeight); 
		~FPSCameraController(); 
		void KeyPressed(int keyCode);
		void KeyReleased(int keyCode);
		void MouseMoved(unsigned int x, unsigned int y, int dx, int dy); 
		
		void Update(double dt); 
		const D3DXMATRIX& GetViewProjection() const;
	private:
		void UpdateRotation(); 
		static const double C_VELOCITY;
		
		int m_movingForward; 
		int m_movingSideways; 
		double m_xyAngle; 
		double m_zAngle; 
		POINT m_lastCursorPos; 
		POINT m_newCursorPos; 
	};
}

#endif