#ifndef _GLM_HELPER_H_
#define _GLM_HELPER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GLMHelper
{
	glm::vec3 rotateAroundX(glm::vec3 v, float degrees);
	glm::vec3 rotateAroundY(glm::vec3 v, float degrees);
	glm::vec3 rotateAroundZ(glm::vec3 v, float degrees);

	glm::vec3 rotateVec3(glm::vec3 v, float degrees, glm::vec3 roAxis);
};

#endif // !_GLM_HELPER_H_

