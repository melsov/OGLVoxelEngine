#include "AACube.h"



namespace VEMath
{
	AACube::AACube()
	{
	}

	AACube::AACube(float _size) : AACube(glm::vec3(0, 0, 0), _size)
	{
	}

	AACube::AACube(glm::vec3 _origin, float _size) : origin(_origin), size(glm::vec3(_size, _size, _size))
	{

		glm::vec3 sx(_size, 0, 0);
		glm::vec3 sy(0, _size, 0);
		glm::vec3 sz(0, 0, _size);
		points[0] = origin;
		points[1] = origin + sx;
		points[2] = origin + sy;
		points[3] = origin + sz;
		points[4] = origin + sx + sz;
		points[5] = origin + sy + sx;
		points[6] = origin + sz + sy;
		points[7] = origin + size;
	}


}

