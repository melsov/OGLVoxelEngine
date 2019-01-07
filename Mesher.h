#pragma once
#ifndef _MESHER_H_
#define _MESHER_H_

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "WorldConfig.h"
#include "RubiksChunk.h"
#include "MeshData.h"
#include "ChunkMesh.h"


namespace ChunkMesh
{

	void CreateMeshM(Cube6::RubiksChunk& rubiks, MeshData& md);

	void AddFaceM(
		int FaceDir,
		unsigned char voxType,
		vec3 relPos,
		unsigned short relIndex,
		MeshData& meshData,
		float scale = 1.0f);

	void AddVFace(
		int FaceDir,
		unsigned char voxType,
		vec3 relPos,
		unsigned short relIndex,
		VFace& vface,
		float scale = 1.0f);

}

#endif // !_MESHER_H_

