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

void MTransformComponent::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
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

const glm::vec3& MTransformComponent::getRotation() const
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
	auto Rx = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	auto Ry = glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	auto Rz = glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	m_worldMat = glm::translate(glm::mat4(1.0f), m_position)
		* glm::mat4(Rz) * glm::mat4(Ry) * glm::mat4(Rx)
		* glm::scale(glm::mat4(1.0f), m_scale);
}
