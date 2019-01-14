#pragma once

#include <glm/glm.hpp>
#include "MathHelper.h"

#define LOG_TWO_CHUNK_SIZE 4
#define CHUNK_SIZE 16
#define HALF_CHUNK_SIZE 8

#define VOXELS_PER_CHUNK (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)

#define IV_CHUNK_SIZE (ivec3(CHUNK_SIZE,CHUNK_SIZE,CHUNK_SIZE))

// worst case chunk: 3d 'checker board'
// for CHUNK_SIZE = 16
// a pair of rows has 8 + 7 voxels
// 8 row pairs per XY slice
// 16 XY slices per chunk
// 6 faces per voxel
// (8 + 7) * 8 * 16 * 6 = 11520
#define MAX_FACES_PER_CHUNK 11520 

// be stingy: in the scenarios we're imagining
// we'll (almost?) never get anywhere near the worst case.
// assume the worst case we'll actually ever see
// is equal to 8 evenly spaced walls (based on we're not sure what)
// each wall has 256 * 2 + 16 * 4 faces
// * 8 walls per chunk
#define PROBABLE_WORST_CASE_FACES_PER_CHUNK 4608

#pragma region LOD

#define NUM_LODS 3

#pragma endregion

//
// World data
//
#define HIGHEST_CHUNK_Y 8
#define LOWEST_CHUNK_Y 0

#define GENERATE_DIM 3
#define IV_GENERATE_BOX (veci3(GENERATE_DIM))

#define UNLOAD_CHUNK_BORDER_WIDTH 2
#define PRESERVE_CHUNK_DIM (GENERATE_DIM + UNLOAD_CHUNK_BORDER_WIDTH)
#define IV_PRESERVE_CHUNK_BOX (veci3(PRESERVE_CHUNK_DIM))

bool inline WorldContainsChunkPos(ivec3 chunkPos)
{
	return chunkPos.y >= LOWEST_CHUNK_Y && chunkPos.y < HIGHEST_CHUNK_Y;
}


namespace NChunkRelative
{
	static glm::ivec3 inline ChunkPosToPos(ivec3 chunkPos)
	{
		return Log2Mult(chunkPos, LOG_TWO_CHUNK_SIZE);
	}


	static ivec3 PosToChunkPos(ivec3 pos, ivec3& relPos)
	{
		auto result = Log2Div(pos, LOG_TWO_CHUNK_SIZE);
		relPos = NegToPosMod(pos, IV_CHUNK_SIZE);
		return result;
	}

	static ivec3 PosToChunkPos(ivec3 pos, veci3& relPos)
	{
		ivec3 irel;
		veci3 result = PosToChunkPos(pos, irel);
		relPos = irel;
		return result;
	}

	static ivec3 PosToChunkPos(ivec3 pos)
	{
		ivec3 irel;
		veci3 result = PosToChunkPos(pos, irel);
		return result;
	}

	static ivec3 PosToChunkPosM(ivec3 pos, ivec3& relPos)
	{
		auto result = Log2Div(pos, LOG_TWO_CHUNK_SIZE);
		relPos = pos - ChunkPosToPos(result); // NegToPosMod(pos, IV_CHUNK_SIZE);
		return result;
	}

	static bool isPosChunkLocal(ivec3 pos)
	{
		return
			pos.x < CHUNK_SIZE && pos.x >= 0 &&
			pos.y < CHUNK_SIZE && pos.y >= 0 &&
			pos.z < CHUNK_SIZE && pos.z >= 0;
	}

	static bool isPosChunkLocal(int x, int y, int z)
	{
		return
			x < CHUNK_SIZE && x >= 0 &&
			y < CHUNK_SIZE && y >= 0 &&
			z < CHUNK_SIZE && z >= 0;
	}
}

