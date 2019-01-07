#include "CamData.h"
#include <iostream>

namespace VECam
{

	void CamData::ComputeMatricesFromInputs()
	{

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		verticalAngle -= mouseSpeed * float(768 / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		
		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		glm::vec3 absup = glm::vec3(0, 1, 0);

		// Up vector
		up = glm::cross(direction, right); // flip order to invert up // glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position -= right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += right * deltaTime * speed;
		}
		// up
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			position += absup * deltaTime * speed;
		}
		// down
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			position -= absup * deltaTime * speed;
		}

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(frustum.fov, frustum.aspect, frustum._near, frustum._far);

		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}

	bool CamData::IsPointInFrustum(glm::vec3 v) const
	{
		auto dif = v - position;

		float dotX, dotY, dotZ;

		dotZ = glm::dot(direction, dif);
		if (dotZ < frustum._near || dotZ > frustum._far) { return false; }

		float fH = dotZ * 2.0f * glm::tan(frustum.fov / 2); 
		dotY = glm::dot(up, dif);
		if (dotY < -fH / 2.0 || dotY > fH / 2.0) { return false; }

		float fW = fH * frustum.aspect;
		dotX = glm::dot(right, dif);
		if (dotX < -fW / 2.0 || dotX > fW / 2.0) { return false; }

		return true;
	}

}