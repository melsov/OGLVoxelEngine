#include "GLMHelper.h"


namespace GLMHelper
{
	glm::vec3 rotateAroundX(glm::vec3 v, float degrees)
	{
		return rotateVec3(v, degrees, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 rotateAroundY(glm::vec3 v, float degrees)
	{
		return rotateVec3(v, degrees, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 rotateAroundZ(glm::vec3 v, float degrees)
	{
		return rotateVec3(v, degrees, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::vec3 rotateVec3(glm::vec3 v, float degrees, glm::vec3 roAxis)
	{
		glm::mat4 ro(1);
		ro = glm::rotate(ro, degrees, roAxis);
		return glm::vec3(ro * glm::vec4(v, 1.0f));
	}
}