#pragma once

#include <glm/glm.hpp>
#include "MathHelper.h"
#include "Chunk.h"
#include "RubiksChunk.h"
#include "ChunkSet.h"
#include "ChunkMesh.h"
#include "IOChunk.h"
#include <iostream>

using namespace glm;

class ChunkBuilder
{
public:
	ChunkBuilder(ivec3 _centerChunkPos) : 
		centerChunkPos(_centerChunkPos) 
	{}

	~ChunkBuilder() {}

	void Generate(VoxelAtFunc voxelAt, ChunkSet& cset);

	void Mesh();

	bool ReadMesh(IOChunk& iochunk);

	ivec3 GetCenterChunkPos() { return centerChunkPos; }

private:
	Cube6::RubiksChunk rubiks;
	ivec3 centerChunkPos;

};

