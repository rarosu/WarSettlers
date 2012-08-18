#include "CameraController.hpp"

namespace Framework 
{ 
	CameraController::CameraController() 
	{

	}

	CameraController::CameraController(Camera *camera)
		: m_camera(camera)
	{
	}


	CameraController::~CameraController()
	{
	}

}