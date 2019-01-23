#pragma once

#include <glm/glm.hpp>

namespace VEMath
{

	struct Edge
	{
		glm::vec3 a, b;

		Edge() {}

		Edge(glm::vec3 aa, glm::vec3 bb)
		{
			a = aa; b = bb;
		}
	};

	class AACube
	{
	public:

		glm::vec3 origin;
		glm::vec3 size;

		glm::vec3 points[8];

		glm::vec3 get(int i) const { return points[i]; }
		glm::vec3& operator[] (int i) { return points[i]; }

		AACube();
		AACube(float _size);
		AACube(glm::vec3 _origin, float _size);

		//ordering per: http://cococubed.asu.edu/code_pages/raybox.shtml
		Edge getEdge(int i) const
		{
			switch (i)
			{
				//light gray
			case 0:
				return Edge(get(0), get(1));
			case 1:
				return Edge(get(1), get(4));
			case 2:
				return Edge(get(4), get(7));

				//dark gray
			case 3:
				return Edge(get(0), get(2));
			case 4:
				return Edge(get(2), get(5));
			case 5: 
				return Edge(get(5), get(7));

				// black
			case 6:
				return Edge(get(0), get(3));
			case 7:
				return Edge(get(3), get(6));
			case 8:
				return Edge(get(6), get(7));

				// dotted light g, dark g, black
			case 9:
				return Edge(get(1), get(5));
			case 10:
				return Edge(get(2), get(6));
			case 11:
			default:
				return Edge(get(3), get(4));
			}
		}

	};

}

