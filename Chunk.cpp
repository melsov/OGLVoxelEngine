#include "Chunk.h"
#include "MathHelper.h"
#include <map>


Chunk::~Chunk()
{
	printf("chunk dtor: ");
	Debug();
}


void Chunk::doGenerateVoxels(VoxelAtFunc voxelAt)
{
	auto voxData = readFromDisk();
	if (voxData != nullptr)
	{
		// todo: set data to this vox data
		// also determine if empty
		return;
	}

	auto cpos = pos;
	ivec3 c;
	bool foundNonAir = false;

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				c = cpos + ivec3(x, y, z);
				Voxel vox = voxelAt(c.x, c.y, c.z);
				data.set(x, y, z, vox);
				if (vox.type > VEMPTY) { foundNonAir = true; }
			}
		}
	}
	empty = !foundNonAir;
}




void Chunk::SetBuildStage(int stage)
{
	buildStage = stage;
}

void Chunk::updateIsEmpty()
{
	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i].type > VEMPTY)
		{
			empty = false;
			return;
		}
	}
	empty = true;
}

void Chunk::generateVoxels(VoxelAtFunc voxelAt)
{
	if (buildStage > BCSNEVER_GENERATED)
	{
		return;
	}

	buildStage = BCSGENERATRING;
	doGenerateVoxels(voxelAt);
	buildStage = BCSHAS_GENERATED;
}

Voxel Chunk::voxelAt(ivec3 pos)
{
	return data.get(pos);
}

bool Chunk::voxelAtSafe(ivec3 pos, Voxel & voxel) 
{
	if (NChunkRelative::isPosChunkLocal(pos))
	{
		voxel = data.get(pos);
		return true;
	}
	return false;
}

bool Chunk::voxelAtSafe(int x, int y, int z, Voxel & voxel)
{
	if (NChunkRelative::isPosChunkLocal(x,y,z))
	{
		voxel = data.get(x,y,z);
		return true;
	}
	return false;
}

void Chunk::Debug()
{
	printf("ChunkDBUG: name: %d addr of flatarr: %x\n", dbugName, &data);
	data.debug();
}


Voxel * Chunk::readFromDisk()
{
	//TODO: read write to disk
	return nullptr;
}


