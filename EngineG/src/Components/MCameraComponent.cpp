#include "MCameraComponent.h"
#include "MTransformComponent.h"
#include "../Entity/MEntity.h"

using namespace MG;

MCameraComponent::MCameraComponent()
{

}

MCameraComponent::~MCameraComponent()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->removeComponent(this);
}

void MCameraComponent::getView(glm::mat4& view) const
{
	auto matWorld = m_entity->getTransform()->getWorldMat();
	glm::vec3 cameraPos = glm::vec3(matWorld[3]);
	glm::vec3 cameraFront = -glm::vec3(matWorld[2]);
	glm::vec3 cameraUp = glm::vec3(matWorld[1]);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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

void MCameraComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->addComponent(this);
}
