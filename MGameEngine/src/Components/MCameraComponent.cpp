#include "MCameraComponent.h"
#include "MTransformComponent.h"
#include "../Entity/MEntity.h"

#include <iostream>

using namespace MG;

MCameraComponent::MCameraComponent()
{
	m_cameraSide = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
}

MCameraComponent::~MCameraComponent()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->removeComponent(this);
}

void MCameraComponent::getView(glm::mat4& view) const
{
	auto matWorld = m_entity->getTransform()->getWorldMat();
	glm::vec3 cameraPos = glm::vec3(matWorld[3]);	
	view = glm::lookAt(cameraPos, cameraPos + m_cameraFront, m_cameraUp);
}

void MCameraComponent::getProjection(glm::mat4& projection) const
{
	projection = m_projection;
}

void MCameraComponent::setFarPlane(f32 farPlane)
{
	m_farPlane = farPlane;
	computeProjection();
}

void MCameraComponent::setNearPlane(f32 nearPlane)
{
	m_nearPlane = nearPlane;
	computeProjection();
}

void MCameraComponent::setFieldOfView(f32 fieldOfView)
{
	m_fieldOfView = fieldOfView;
	computeProjection();
}

void MCameraComponent::setScreenArea(const MRect& screenArea)
{
	m_screenArea = screenArea;
	computeProjection();
}

void MCameraComponent::setType(const MCameraType type)
{
	m_type = type;
	computeProjection();
}

f32 MCameraComponent::getFarPlane() const
{
	return m_farPlane;
}

f32 MCameraComponent::getNearPlane() const
{
	return m_nearPlane;
}

f32 MCameraComponent::getFieldOfView() const
{
	return m_fieldOfView;
}

MCameraType MCameraComponent::getType()
{
	return m_type;
}

void MCameraComponent::computeProjection()
{
	if (m_type == MCameraType::Perspective)
	{
		auto aspect = (float)m_screenArea.width / (float)m_screenArea.height;
		m_projection = glm::perspective(m_fieldOfView, aspect, m_nearPlane, m_farPlane);
	}
}

void MCameraComponent::rotateCameraPitch(float delta)
{
	if (!m_readInput) return;

	auto R = glm::rotate(glm::mat4(1.0f), delta, glm::normalize(m_cameraSide));
	
	auto cameraFront = glm::mat3(R) * m_cameraFront;

	auto frontDotUp = glm::dot(-cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

	if (frontDotUp > -0.7071 && frontDotUp < 0.7071)
	{
		m_cameraFront = glm::mat3(R) * m_cameraFront;

		m_cameraSide = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -m_cameraFront));
		m_cameraUp = glm::normalize(glm::cross(-m_cameraFront, m_cameraSide));
	}
}

void MCameraComponent::rotateCameraYaw(float delta)
{
	if (!m_readInput) return;

	auto R = glm::rotate(glm::mat4(1.0f), -delta, glm::vec3(0.0f, 1.0f, 0.0f));
	m_cameraFront = glm::mat3(R) * m_cameraFront;

	m_cameraSide = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -m_cameraFront));
	m_cameraUp = glm::normalize(glm::cross(-m_cameraFront, m_cameraSide));
}

void MCameraComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->addComponent(this);
}

const glm::vec3& MCameraComponent::getLookAt() const
{
	return m_cameraFront;
}

void MG::MCameraComponent::setReadInput(bool readInput)
{
	m_readInput = readInput;
}

const glm::vec3& MCameraComponent::getSide() const
{
	return m_cameraSide;
}
