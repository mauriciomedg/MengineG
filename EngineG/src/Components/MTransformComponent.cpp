#include "MTransformComponent.h"

using namespace MG;

MTransformComponent::MTransformComponent()
{
}

MTransformComponent::~MTransformComponent()
{
}

void MTransformComponent::setPosition(const glm::vec3& position)
{
	m_position = position;
	updateWorldMat();
}

void MTransformComponent::setRotation(float angle, const glm::vec3& axis)
{
	m_rotation = glm::angleAxis(angle, axis);
	m_rotation = glm::normalize(m_rotation);
	updateWorldMat();
}

void MTransformComponent::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateWorldMat();
}

const glm::vec3& MTransformComponent::getPosition() const
{
	return m_position;
}

const glm::quat& MTransformComponent::getRotation() const
{
	return m_rotation;
}

const glm::vec3& MTransformComponent::getScale() const
{
	return m_scale;
}

void MTransformComponent::setWorldMat(const glm::mat4& worldMat)
{
	m_worldMat = worldMat;
}

const glm::mat4& MTransformComponent::getWorldMat() const
{
	return m_worldMat;
}

void MTransformComponent::updateWorldMat()
{
	auto R = glm::mat4(m_rotation);
	m_worldMat = glm::translate(glm::mat4(1.0f), m_position)
		* R
		* glm::scale(glm::mat4(1.0f), m_scale);
}
