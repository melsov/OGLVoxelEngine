#pragma once
#ifndef _CAM_DATA_H_
#define _CAM_DATA_H_

#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VECam
{


	struct Frustum
	{
		float fov;
		float aspect;
		float _near;
		float _far;
	};

	class CamData
	{
	public:
		CamData(GLFWwindow* _window) :
			window(_window)
		{
		}
		~CamData() {}

		glm::mat4 getViewMatrix() const
		{
			return ViewMatrix;
		}

		glm::mat4 getProjectionMatrix() const
		{
			return ProjectionMatrix;
		}

		glm::vec3 getPosition() const
		{
			return position;
		}

		glm::vec3 getDirection() const
		{
			return direction;
		}

		float speed = 20.0f;
		float mouseSpeed = 0.005f;

		Frustum frustum;

		void ComputeMatricesFromInputs();

		bool IsPointInFrustum(glm::vec3 v) const;

	private:
		GLFWwindow* window;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::vec3 position;
		glm::vec3 direction, right, up;

		float horizontalAngle, verticalAngle;

	};


	static void SetupDefaultCam(CamData& cam)
	{
		cam.frustum.fov = glm::radians(45.0f);
		cam.frustum.aspect = 4.0f / 3.0f;
		cam.frustum._near = 0.01f;
		cam.frustum._far = 100.0f;
	}
}
#endif // !_CAM_DATA_H_
