#ifndef FRAMEWORK_CAMERA_HPP
#define FRAMEWORK_CAMERA_HPP

#include "D3DInclude.hpp"

namespace Framework
{
	struct Frustum
	{
		D3DXVECTOR3 m_position;
		D3DXVECTOR3 m_direction;
		double m_near;
		double m_far;
		double m_fieldOfViewY;
		double m_aspectRatio;

		D3DXMATRIX CalculateProjection() const;
	};


	class Camera
	{
	public:
		Camera(); 
		Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& target, const D3DXMATRIX& projection);
		Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& target, double _near, double _far, double fieldOfViewY, double aspectRatio); 

		void SetPosition(const D3DXVECTOR3& position);
		void SetDirection(const D3DXVECTOR3& direction);
		void SetTargetPoint(const D3DXVECTOR3& target);

		const D3DXVECTOR3& GetPosition() const;
		const D3DXVECTOR3& GetDirection() const;

		void SetProjection(const D3DXMATRIX& projection);

		void Commit();

		const D3DXMATRIX& GetView() const;
		const D3DXMATRIX& GetProjection() const;
		const D3DXMATRIX& GetViewProjection() const;
	private:
		D3DXVECTOR3 m_position;
		D3DXVECTOR3 m_direction;
		
		D3DXMATRIX m_view;
		D3DXMATRIX m_projection;
		D3DXMATRIX m_viewProjection;
		Framework::Frustum m_frustum; 
	};
}

#endif