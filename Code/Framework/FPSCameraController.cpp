#include "FPSCameraController.hpp"

namespace Framework
{	
	const double FPSCameraController::C_VELOCITY = 10;
	const double FPSCameraController::C_SENSITIVITY = 1;

	FPSCameraController::FPSCameraController()
	{

	}

	FPSCameraController::FPSCameraController(Camera *camera, int screenWidth, int screenHeight)
		: CameraController(camera, screenWidth, screenHeight) 
		, m_movingForward(0)
		, m_movingSideways(0)		
	{
		Framework::InputManager::Instance().AddInputListener(this);			
	}
	
	FPSCameraController::~FPSCameraController()
	{
		Framework::InputManager::Instance().RemoveInputListener(this);		
	}

	void FPSCameraController::KeyPressed(int keyCode)
	{
		switch(keyCode) 
		{
			case VK_UP:
				m_movingForward = 1; 
			break; 

			case VK_DOWN: 
				m_movingForward = -1; 
			break; 

			case VK_RIGHT:
				m_movingSideways = 1; 
			break; 

			case VK_LEFT: 
				m_movingSideways = -1; 
			break; 
		}
	}

	void FPSCameraController::KeyReleased(int keyCode)
	{
		switch(keyCode) 
		{
			case VK_UP:
			case VK_DOWN: 
				m_movingForward = 0; 
			break; 

			case VK_RIGHT:
			case VK_LEFT: 
				m_movingSideways = 0;  
			break; 
		}
	}

	
	void FPSCameraController::Update(double dt)
	{	
		D3DXVECTOR3 position = m_camera->GetPosition();
		D3DXVECTOR3 direction = m_camera->GetDirection();		
	
		UpdateRotation(direction, dt); 
					
		m_camera->SetPosition(position + C_VELOCITY * dt * direction * m_movingForward); 						
	}

	void FPSCameraController::UpdateRotation(D3DXVECTOR3 direction, double dt)
	{		
		POINT cursorPos; 
		GetCursorPos(&cursorPos);
		
		int clientWidth = Framework::Window::Instance().GetClientWidth(); 
		int clientHeight = Framework::Window::Instance().GetClientHeight(); 
		
		int dx = clientWidth / 2 - cursorPos.x;
		int dy = clientHeight / 2 - cursorPos.y;
		
		float radius = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		float yaw = std::atan2(direction.z, direction.x);
		float pitch = std::asin(direction.y / radius);

		yaw += dx * C_SENSITIVITY * dt;
		pitch += dy * C_SENSITIVITY * dt;
					
		if(pitch > M_PI_4) 
			pitch = M_PI_4; 
		if(pitch < -M_PI_4) 
			pitch = -M_PI_4; 

		float aux = radius * std::cos(pitch);
		direction.x = aux * std::cos(yaw);
		direction.y = radius * std::sin(pitch);
		direction.z = aux * std::sin(yaw);
					
		m_camera->SetDirection(direction); 		

		SetCursorPos(clientWidth/2, clientHeight/2); 			
	}

	const D3DXMATRIX& FPSCameraController::GetViewProjection() const
	{
		return m_camera->GetViewProjection(); 
	}

}



