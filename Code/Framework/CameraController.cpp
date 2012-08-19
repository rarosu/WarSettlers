#include "CameraController.hpp"

namespace Framework 
{ 
	CameraController::CameraController() 
	{

	}

	CameraController::CameraController(Camera *camera, int screenWidth, int screenHeight)
		: m_camera(camera)
		, m_screenWidth(screenWidth)
		, m_screenHeight(screenHeight)
	{
		Framework::Window::Instance().AddEventListener(this); 
	}


	CameraController::~CameraController()
	{
		Framework::Window::Instance().RemoveEventListener(this); 
	}

	void CameraController::WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight)
	{
		m_camera->UpdateAspectRatio((double)clientWidth/(double)clientHeight); 
		m_screenWidth = clientWidth; 
		m_screenHeight = clientHeight; 
	}


}