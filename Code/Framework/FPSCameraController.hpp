#ifndef FRAMEWORK_FPS_CAMERACONTROLLER_HPP
#define FRAMEWORK_FPS_CAMERACONTROLLER_HPP

#define _USE_MATH_DEFINES

#include <cmath>
#include "Window.hpp"
#include "InputManager.hpp"
#include "CameraController.hpp"
#include "Camera.hpp"

namespace Framework
{
	class FPSCameraController : public CameraController, public Framework::InputListener
	{
	public:
		FPSCameraController(); 
		FPSCameraController(Camera *camera, int screenWidth, int screenHeight); 
		~FPSCameraController(); 
		void KeyPressed(int keyCode);
		void KeyReleased(int keyCode);
		
		void Update(double dt); 
		const D3DXMATRIX& GetViewProjection() const;
	private:
		void UpdateRotation(D3DXVECTOR3 direction, double dt); 
		static const double C_VELOCITY;
		static const double C_SENSITIVITY;
		
		int m_movingForward;
		int m_movingSideways;
	};
}

#endif