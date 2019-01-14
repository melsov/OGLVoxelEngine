#pragma once
#include <vector>
#include "MathHelper.h"
#include "WorldConfig.h"
#include <stdio.h>

namespace IDirections
{
	class LODFaceCoords
	{



	public:
		LODFaceCoords(int lod)
		{
			Init(lod);
		}

		std::vector<veci3>& operator[](int fd)
		{
			return coords[fd];
		}

	//private:
		std::vector<veci3> coords[6];

		void Init(int lod)
		{
			for (int dir = 0; dir < FD_NUM_FACE_DIRECTIONS; ++dir)
			{
				veci3 cursor, perpA, perpB;
				cursor = DirForFaceDir(dir);
				perpA = PerpDirForFaceDir(dir, false);
				perpB = PerpDirForFaceDir(dir, true);

				int dim = pow(2, lod);
				veci3 a, b;
				for (int i = 0; i < dim; ++i)
				{
					a = perpA * i;
					for (int j = 0; j < dim; ++j)
					{
						b = perpB * j;
						coords[dir].push_back(cursor + a + b);
					}
				}
			}
		}
	};

	class LODFaceLookup
	{

	public:
		std::vector<LODFaceCoords> faces;

		LODFaceLookup()
		{
			for (int i = 0; i < NUM_LODS; ++i)
			{
				faces.push_back(LODFaceCoords(i));
			}
		}

	};

	static LODFaceLookup LodFaceLookup;
}