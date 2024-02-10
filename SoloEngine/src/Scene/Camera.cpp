
#include "Camera.h"

namespace Solo
{
	Camera::Camera()
	{
		setProjectionPerspective(fovd_, aspectRatio_, nearClip_, farClip_);
	}

	Camera::Camera(float fovd, float aspectRatio, float nearClip, float farClip)
		: fovd_(fovd)
		, aspectRatio_(aspectRatio)
		, nearClip_(nearClip)
		, farClip_(farClip)
	{
		setProjectionPerspective(fovd_, aspectRatio_, nearClip_, farClip_);
	}
	void Camera::setProjectionOrtho(float left, float right, float bottom, float top)
	{
		projection_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		recalculateViewMatrix();
	}

	void Camera::setProjectionPerspective(float fovyd, float aspect, float near, float far)
	{
		projection_ = glm::perspective(glm::radians(fovyd), aspect, near, far);
		recalculateViewMatrix();
	}

	void Camera::recalculateViewMatrix()
	{
		// The view matrix represents the transformations required to take the world
		// and put it infront of the camera. You can get this by taking the inverse of
		// the camera's transformation in worldspace
		glm::mat4 invView = glm::translate(glm::mat4(1.0f), position_)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotY_), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotX_), glm::vec3(1, 0, 0))
			;

		view_ = glm::inverse(invView);
		viewProjection_ = projection_ * view_;
	}
}
