#ifndef FRAMEWORK_CAMERACONTROLLER_HPP
#define FRAMEWORK_CAMERACONTROLLER_HPP

#include "Camera.hpp" 

namespace Framework
{

	class CameraController
	{
	public:
		CameraController(); 
		CameraController(Camera *camera);
		virtual ~CameraController(); 

		virtual void Update(double dt) = 0; 
	protected: 
		Camera *m_camera; 
	};

}
#endif