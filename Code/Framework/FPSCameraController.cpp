#include "FPSCameraController.hpp"

namespace Framework
{	
	const double FPSCameraController::C_VELOCITY = 10; 

	FPSCameraController::FPSCameraController()
	{

	}

	FPSCameraController::FPSCameraController(Camera *camera, int screenWidth, int screenHeight)
		: CameraController(camera, screenWidth, screenHeight) 
		, m_movingForward(0)
		, m_movingSideways(0)
		, m_xyAngle(0.0)
		, m_zAngle(0.0)
	{
		Framework::InputManager::Instance().AddInputListener(this);		
		ZeroMemory(&m_lastCursorPos, sizeof(POINT)); 
		ZeroMemory(&m_newCursorPos, sizeof(POINT)); 
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

			
	void FPSCameraController::MouseMoved(unsigned int x, unsigned int y, int dx, int dy)
	{			
			
		
	}
	
	void FPSCameraController::Update(double dt)
	{		
		UpdateRotation(); 

		D3DXVECTOR3 position = m_camera->GetPosition();
		D3DXVECTOR3 direction = m_camera->GetDirection();		
					
		m_camera->SetPosition(position + C_VELOCITY * dt * direction * m_movingForward); 				
		m_camera->Commit(); 
	}

	void FPSCameraController::UpdateRotation()
	{
		double dx = 0.0; 
		double dy = 0.0; 

		GetCursorPos(&m_newCursorPos); 

		if(m_newCursorPos.x != m_screenWidth/2 
			&& m_newCursorPos.y != m_screenHeight/2) 
		{			
			dx = m_newCursorPos.x - m_lastCursorPos.x; 
			dy = m_newCursorPos.y - m_lastCursorPos.y;		

			m_xyAngle += (double)dx / 1000.0; 
			m_zAngle += (double)dy / 1000.0; 

			if(m_zAngle > M_PI_4) 
				m_zAngle = M_PI_4; 
			if(m_zAngle < -M_PI_4) 
				m_zAngle = -M_PI_4; 

			if(m_xyAngle > M_PI_2) 
				m_xyAngle = M_PI_2; 
			if(m_xyAngle < -M_PI_2) 
				m_xyAngle = -M_PI_2; 

			double sx = cos((double)m_xyAngle) * sin((double)m_zAngle); 
			double sz = sin((double)m_xyAngle) * sin((double)m_zAngle); 
			double sy = cos((double)m_zAngle); 
		
			m_camera->SetDirection(D3DXVECTOR3(sx, sy, sz)); 
		}

		SetCursorPos(m_screenWidth/2, m_screenHeight/2); 			
		m_lastCursorPos = m_newCursorPos; 					
	}

	const D3DXMATRIX& FPSCameraController::GetViewProjection() const
	{
		return m_camera->GetViewProjection(); 
	}

}