#pragma once
#include "../AllMHeaders.h"
#include <glm/gtc/matrix_transform.hpp>

namespace MG
{
	class MTransformComponent : public MComponent
	{
	public:
		MTransformComponent();
		virtual ~MTransformComponent();

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setScale(const glm::vec3& scale);

		const glm::vec3& getPosition() const;
		const glm::vec3& getRotation() const;
		const glm::vec3& getScale() const;

		void setWorldMat(const glm::mat4& worldMat);
		const glm::mat4& getWorldMat() const;

	protected:
		void updateWorldMat();

	protected:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::mat4 m_worldMat;

	};
}