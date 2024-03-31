#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Solo
{
	class Camera
	{
	public:
		Camera(); // Defaults are set to reasonable values
		Camera(float fovd, float aspectRatio, float nearClip, float farClip);

		void setProjectionOrtho(float left, float right, float bottom, float top);

		void setProjectionPerspective(float fovyd, float aspect, float near, float far);

		inline const glm::vec3& getPosition() const noexcept
		{
			return position_;
		}

		inline void setPosition(const glm::vec3& position)
		{
			position_ = position;
			recalculateViewMatrix();
		}

		inline void addRotation(float rotdxd, float rotdyd)
		{
			rotX_ += rotdxd;
			rotY_ += rotdyd;
			recalculateViewMatrix();
		}

		inline void setRotation(float rotxd, float rotyd)
		{
			rotX_ = rotxd;
			rotY_ = rotyd;
			recalculateViewMatrix();
		}

		inline const glm::vec2& getRotationXY() const noexcept
		{
			return { rotX_, rotY_ };
		}

		inline const glm::mat4& getProjection() const noexcept
		{
			return projection_;
		}

		inline const glm::mat4& getView() const noexcept
		{
			return view_;
		}

		inline const glm::mat4& getViewProjection() const noexcept
		{
			return viewProjection_;
		}
	private:

		// Recalculate matrices
		void recalculateViewMatrix();


		// Camera Parameters
		float fovd_ = 60.0f;
		float aspectRatio_ = 1.778f;
		float nearClip_ = 0.1f;
		float farClip_ = 1000.0f;

		// Transformations
		glm::vec3 position_ = {0.0f,8.0f,0.0f};
		glm::vec3 focalPoint_ = {0.0f,0.0f,0.0f};
		
		float rotX_ = 0.0f;
		float rotY_ = 90.0f;
		float rotZ_ = 0.0f;

		glm::mat4 projection_ = {};
		glm::mat4 view_ = {};
		glm::mat4 viewProjection_ = {};


	};
}
