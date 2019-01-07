#pragma once
#ifndef _MORTON_CHUNK_H_
#define _MORTON_CHUNK_H_

#include "morton.h"
#include <stdint.h>
#include "MathHelper.h"
#include "WorldConfig.h"
#include <stdio.h>

// credit: https://github.com/Forceflow/libmorton

namespace MortonChunk
{

	static uint_fast32_t encodeU32(const uint_fast16_t x, const uint_fast16_t y, const uint_fast16_t z)
	{
		return libmorton::morton3D_32_encode(x, y, z);
	}

	static uint_fast32_t encodeU32(const veci3 v)
	{
		return libmorton::morton3D_32_encode(v.x, v.y, v.z);
	}

	static void decodeU32(const uint_fast32_t morton, uint_fast16_t& x, uint_fast16_t& y, uint_fast16_t& z)
	{
		libmorton::morton3D_32_decode(morton, x, y, z);
	}


	static bool IsDifferentMortonCube(const veci3& a, const veci3& b, int cubeLog2)
	{
		return !((a >> cubeLog2) == (b >> cubeLog2));
	}

	struct MortonChunkCoords
	{
	private:
		veci3 coords[VOXELS_PER_CHUNK];
		void Init()
		{
			veci3 pos;

			for (int x = 0; x < CHUNK_SIZE; ++x)
			{
				pos.x = x;
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					pos.y = y;
					for (int z = 0; z < CHUNK_SIZE; ++z)
					{
						pos.z = z;
						auto mindex = encodeU32(pos);
						coords[mindex] = pos;
					}
				}
			}
		}

	public:
		MortonChunkCoords()
		{
			Init();
		}

		const veci3& operator[] (int i) const
		{
			return coords[i];
		}

		void DebugPrintSomeCoords() const
		{
			/*for (int i = 0; i < 8; ++i)
			{
				auto v = coords[i];
				std::cout << "index: " << i << " : " << "v: " << v.x <<  ", " << v.y << ", " << v.z << std::endl;
			}

			for (int i = VOXELS_PER_CHUNK - 8; i < VOXELS_PER_CHUNK; ++i)
			{
				auto v = coords[i];
				std::cout << "index: " << i << " : " << "v: " << v.x  << ", " << v.y << ", " << v.z << std::endl;
			}*/
		}
	};

	static MortonChunkCoords ChunkCoords;
}

#endif // !_MORTON_CHUNK_H_
