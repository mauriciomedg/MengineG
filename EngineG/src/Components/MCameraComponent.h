#pragma once

#include "../OGraphicsEngine/MPrerequisites.h"
#include "../AllMHeaders.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MG
{
	struct MRect
	{
		f32 width;
		f32 height;
	};

	class MCameraComponent : public MComponent
	{
	public:
		MCameraComponent();
		virtual ~MCameraComponent();

		void getView(glm::mat4& view) const;
		void getProjection(glm::mat4& projection) const;

		void setFarPlane(f32 farPlane);
		void setNearPlane(f32 nearPlane);
		void setFieldOfView(f32 fieldOfView);
		void setScreenArea(const MRect& screenArea);
		void setType(const MCameraType type);

		f32 getFarPlane() const;
		f32 getNearPlane() const;
		f32 getFieldOfView() const;
		MCameraType getType();

		void rotateCameraPitch(float delta);
		void rotateCameraYaw(float delta);

		const glm::vec3& getLookAt() const;
		const glm::vec3& getSide() const;

	protected:
		virtual void onCreateInternal() override;

	private:
		void computeProjection();
	private:

		glm::mat4 m_projection;
		
		f32 m_nearPlane = 0.01f;
		f32 m_farPlane = 1000.0f;
		f32 m_fieldOfView = 1.3f;
		MRect m_screenArea = {};
		MCameraType m_type = MCameraType::Perspective;

		glm::vec3 m_cameraSide;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraFront;

		
	};
}