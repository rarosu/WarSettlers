#include "Camera.hpp"

namespace Framework
{
	D3DXMATRIX Frustum::CalculateProjection() const
	{
		D3DXMATRIX projection;
	
		D3DXMatrixPerspectiveFovLH(&projection, m_fieldOfViewY, m_aspectRatio, m_near, m_far);
	
		return projection;
	}



	Camera::Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& target, const D3DXMATRIX& projection)
		: m_position(position)
	{
		SetTargetPoint(target);
		SetProjection(projection);
	}

	void Camera::SetPosition(const D3DXVECTOR3& position)
	{
		m_position = position;
	}

	void Camera::SetDirection(const D3DXVECTOR3& direction)
	{
		m_direction = direction;
		D3DXVec3Normalize(&m_direction, &m_direction);
	}

	void Camera::SetTargetPoint(const D3DXVECTOR3& target)
	{
		SetDirection(target - m_position);
	}


	const D3DXVECTOR3& Camera::GetPosition() const
	{
		return m_position;
	}

	const D3DXVECTOR3& Camera::GetDirection() const
	{
		return m_direction;
	}


	void Camera::SetProjection(const D3DXMATRIX& projection)
	{
		m_projection = projection;
	}


	void Camera::Commit()
	{
		const D3DXVECTOR3 C_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXVECTOR3 target = m_position + m_direction; 
		D3DXMatrixLookAtLH(&m_view, &m_position, &target, &C_UP); 

		m_viewProjection = m_view * m_projection;
	}


	const D3DXMATRIX& Camera::GetView() const
	{
		return m_view;
	}

	const D3DXMATRIX& Camera::GetProjection() const
	{
		return m_projection;
	}

	const D3DXMATRIX& Camera::GetViewProjection() const
	{
		return m_viewProjection;
	}

}