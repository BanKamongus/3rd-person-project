#pragma once

#include "../Camera.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(GameObject& gameObject)
		: m_target(gameObject)
	{
	}

	virtual void Update(float dt) override
	{
		const glm::vec3 offset = glm::vec3(0, 2, -3);
		const glm::vec3 targetPosition = m_target.m_position;
		m_position = targetPosition + offset;
		m_lookAt = targetPosition;
		m_lookAt.y += 1;
	}

protected:
	GameObject& m_target;
};