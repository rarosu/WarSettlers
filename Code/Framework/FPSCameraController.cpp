#include "FPSCameraController.hpp"

namespace Framework
{
	const double FPSCameraController::C_VELOCITY = 10; 

	FPSCameraController::FPSCameraController()
	{

	}

	FPSCameraController::FPSCameraController(Camera *camera)
		: CameraController(camera) 
		, m_movingForward(0)
		, m_movingSideways(0)
	{
		Framework::InputManager::Instance().AddInputListener(this);
		
	}
	
	FPSCameraController::~FPSCameraController()
	{
		Framework::InputManager::Instance().RemoveInputListener(this);
		Framework::Window::Instance().RemoveEventListener(this); 
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

			
	void FPSCameraController::MouseMoved(unsigned int x, unsigned int y, int dx, int dy)
	{

	}
	
	void FPSCameraController::Update(double dt)
	{
		D3DXVECTOR3 position = m_camera->GetPosition();
		D3DXVECTOR3 direction = m_camera->GetDirection();
	
		m_camera->SetPosition(position + C_VELOCITY * dt * direction * m_movingForward); 				
		m_camera->Commit(); 
	}

	const D3DXMATRIX& FPSCameraController::GetViewProjection() const
	{
		return m_camera->GetViewProjection(); 
	}

}