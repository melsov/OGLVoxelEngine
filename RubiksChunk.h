#pragma once
#ifndef _RUBIKS_CHUNK_H_
#define _RUBIKS_CHUNK_H_

#include "Chunk.h"
#include "MathHelper.h"
#include "WorldConfig.h"
#include <iostream>

namespace Cube6
{
	class RubiksChunk
	{
	private:

#pragma region _private

		Chunk* storage[27];

		int inline IndexFor(ivec3 v)
		{
			return IndexFor(v.x, v.y, v.z);
		}

		int IndexFor(int x, int y, int z)
		{
			return ++x * 9 + ++y * 3 + ++z;
		}

#pragma endregion

	public:

#pragma region chunk-access

		RubiksChunk() : storage() {}

		~RubiksChunk() {}

		Chunk* get(ivec3 v) {
			//NOTE: we are getting a nonsense value here: (x = -53687092 ?)
			return storage[IndexFor(v)]; }

		Chunk* get(int x, int y, int z)
		{
			return storage[IndexFor(x, y, z)];
		}

		void set(ivec3 v,  Chunk* val)
		{
			storage[IndexFor(v)] = val;
		}

		void set(int x, int y, int z,  Chunk* val)
		{
			storage[IndexFor(x, y, z)] = val;
		}

		Chunk* getWithFaceDir(int fd)
		{
			return get(IDirections::DirForFaceDir(fd));
		}

		void setWithFaceDir(int fd, Chunk*& val)
		{
			set(IDirections::DirForFaceDir(fd), val);
		}

		Chunk* down() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::DOWN]); }
		Chunk* up() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::UP]); }
		Chunk* left() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::LEFT]); }
		Chunk* right() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::RIGHT]); }
		Chunk* back() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::BACK]); }
		Chunk* forward() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::FORWARD]); }
		Chunk* center() { return get(IDirections::zeroi); }

#pragma endregion

#pragma region voxel-access
		
		// relPos relative to center chunk 0, 0, 0
		Voxel voxelAt(ivec3 relPos, Chunk* chunk)
		{
			ivec3 cpos, rel;
			cpos = NChunkRelative::PosToChunkPos(relPos, rel);
			chunk = get(cpos);
			return chunk->voxelAt(rel);
		}

		// relPos relative to center chunk 0, 0, 0
		Voxel voxelAt(ivec3 relPos)
		{
			ivec3 cpos, rel;
			cpos = NChunkRelative::PosToChunkPos(relPos, rel);
			auto chunk = get(cpos);
			return chunk->voxelAt(rel);
		}

		bool voxelAtSafe(int x, int y, int z, Voxel& voxel)
		{
			veci3 cpos, rel;
			cpos = NChunkRelative::PosToChunkPosVecI3(x, y, z, rel); // PosToChunkPos(relPos, rel);
			auto chunk = get(cpos.x, cpos.y, cpos.z);
			if (chunk != nullptr)
			{
				voxel = chunk->voxelAt(rel);
				return true;
			}

			return false;
		}

		bool voxelAtSafe(ivec3 relPos, Voxel& voxel)
		{
			ivec3 cpos, rel;
			cpos = NChunkRelative::PosToChunkPos(relPos, rel);
			auto chunk = get(cpos);
			if (chunk != nullptr)
			{
				voxel = chunk->voxelAt(rel);
				return true;
			}
			
			return false;
		}

#pragma endregion

#pragma region assert-check

		bool hasSixFaceChunksAndCenter()
		{
			bool result = center() != nullptr;
			for (int i = IDirections::FD_DOWN; i < IDirections::FD_NUM_FACE_DIRECTIONS; ++i)
			{
				if (get(IDirections::DirForFaceDir(i)) == nullptr)
				{
					result = false;
					break;
				}
			}
			if (!result)
			{
				printf("failed 6 face check\n");
			}
			return result;
		}

#pragma endregion
	};
}

#endif // !_RUBIKS_CHUNK_H_
