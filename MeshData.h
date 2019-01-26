#pragma once
#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "WorldConfig.h"


typedef unsigned short TRI_T;

namespace ChunkMesh
{
	struct BufferHandles
	{
		GLuint verts;
		GLuint tris;
	};

	struct VVertex
	{
		GLfloat pos[3];
		GLfloat uvs[2];
		GLfloat color[4];
	};

	bool operator==(const VVertex& lhs, const VVertex& rhs);
	



	std::ostream& operator<<(std::ostream &stream, const VVertex v);

	static VVertex FakeVVertex()
	{
		VVertex v;
		v.pos[0] = 3;
		v.pos[1] = 15.9;
		v.pos[2] = 4;
		v.uvs[0] = .76f;
		v.uvs[1] = .0625;
		v.color[0] = .0013;
		return v;
	}


	// Used in mesher.cpp
	struct VFace
	{
		VVertex verts[4];
		TRI_T tris[6];
	};

	struct MeshData
	{
		int lodTriOffsets[NUM_LODS];
		std::vector<VVertex> verts;
		std::vector<TRI_T> tris;


		int triCountAtLOD(int lodPow2)
		{
			return lodTriOffsets[NUM_LODS - 1 - lodPow2];
		}

		void debug()
		{
			printf("vverts size: %d , uvs size:  , tris %d. \n verts.size mod 4 %d \n", 
				verts.size(), tris.size(), verts.size() % 4);
		}
	};

	bool AssertEqual(MeshData& a, MeshData& b);

	class LODMeshDataSet
	{
		
	public:		
		MeshData mds[NUM_LODS];
	};
}





#endif // !_MESH_DATA_H_
