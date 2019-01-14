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
		//GLuint uvs;
		GLuint tris;
	};

	struct VVertex
	{
		GLfloat pos[3];
		GLfloat uvs[2];
		GLfloat color[4];
	};

	struct VFace
	{
		VVertex verts[4];
		TRI_T tris[6];
	};

	struct MeshData
	{
		int lodTriOffsets[NUM_LODS];
		int triCountAtLOD(int lodPow2)
		{
			// vertices are in low lod to high lod order
			// but lodPow2 = 0 corresponds to max lod
			int lodCount = NUM_LODS - lodPow2;
			int r = 0;
			for (int i = 0; i < lodCount; ++i) r += lodTriOffsets[i];
			return r;
		}
		std::vector<VVertex> verts;
		///std::vector<GLfloat> uvs;
		std::vector<TRI_T> tris;

		//BufferHandles handles;

		void debug()
		{
			printf("vverts size: %d , uvs size:  , tris %d. \n verts.size mod 4 %d \n", 
				verts.size(), tris.size(), verts.size() % 4);
		}
	};

	class LODMeshDataSet
	{
		
	public:		
		MeshData mds[NUM_LODS];
	};
}

#endif // !_MESH_DATA_H_
