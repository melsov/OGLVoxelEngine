#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "FlatPow2Array3D.h"
#include "WorldConfig.h"
#include "MeshData.h"
#include <stdio.h>
#include <memory>

/*
Stages of a chunk's life:
--Created, unpopulated
--possibly: decoration requests added (trees, structures, caves)
--populated
--surrounded: 6 neighbors populated
--meshed
--possible:
--edited: decorated, remeshed
--edited: user adds, remeshed
--hopefully saved at some point
--destroyed
*/

using namespace glm;

enum VoxelType
{
	VNEVER_ASSIGNED, 
	VEMPTY, 
	VDIRT, 
	VGRASS, 
};

enum BuildChunkStage
{
	BCSNEVER_GENERATED,
	BCSGENERATRING,
	BCSHAS_GENERATED,
};

struct Voxel 
{
	unsigned char type;

	char* ToString()
	{
		char buffer[50];
		sprintf_s(buffer, "Voxel: %d", type);
		return buffer;
	}
};

typedef Voxel(*VoxelAtFunc)(int, int, int);

class Chunk
{
public:

	Chunk(ivec3 _chunkPos) :
		data(FlatPow2Array3D<Voxel>(LOG_TWO_CHUNK_SIZE)),
		chunkPos(_chunkPos),
		pos(NChunkRelative::ChunkPosToPos(_chunkPos)),
		dirty(true)
	{
		Init();
	}

	//
	// Operator equals and copy constructor not allowed
	// since we don't want to have to deep copy data.
	// copying data by reference leads to calling
	// delete[] twice.
	//
	Chunk& operator=(Chunk other) = delete;
	Chunk(const Chunk& other) = delete;
	

	~Chunk() 
	{
		printf("chunk dtor: ");
		Debug();
	}

	int dbugName;


	ChunkMesh::MeshData meshData;

	ChunkMesh::MeshData* getMeshData(int lod = 0)
	{
		return &meshData;
		//return &meshSet.mds[lod]; // &meshData;
	}
	//ChunkMesh::MeshData& meshData(int lod)
	//{
	//	return meshSet.mds[lod];
	//}

	//ChunkMesh::LODMeshDataSet meshSet;
	//ChunkMesh::MeshData meshData;

	ivec3 position() { return pos; }
	vec3 positionV3() { return vec3(pos.x, pos.y, pos.z); }
	mat4 getModelMatrix()
	{
		return glm::translate(mat4(1.0f), positionV3());
	}
	ivec3 chunkPosition() { return chunkPos; }

	virtual bool getIsEmpty() { return empty; }
	virtual bool getIsDirty() { return dirty; }
	void setDirty(bool _dirty) { dirty = _dirty; }

	virtual void generateVoxels(VoxelAtFunc voxelAt);
	virtual Voxel voxelAt(ivec3 pos);
	virtual bool voxelAtSafe(ivec3 pos, Voxel& voxel);
	virtual bool isAVoidChunk() { return false; }

	void Debug();


protected:
	Chunk() : 
		data(FlatPow2Array3D<Voxel>(0)) 
	{
		printf("(chunk default ctor) \n");
	}

	void Init()
	{
		dbugName = -1;
		empty = true;
		buildStage = BCSNEVER_GENERATED;
	}

	void doGenerateVoxels(VoxelAtFunc voxelAt);
	Voxel* readFromDisk();
	 
	FlatPow2Array3D<Voxel> data;
	ivec3 chunkPos;
	ivec3 pos;
	bool empty;
	bool dirty;
	int buildStage;
};

class VoidChunk : public Chunk
{
public:
	VoidChunk(ivec3 _chunkPos) : Chunk() 
	{
		chunkPos = _chunkPos;
		pos = NChunkRelative::ChunkPosToPos(_chunkPos);
	}

	bool getIsEmpty() { return true; }
	bool getIsDirty() { return false; }

	void generateVoxels(VoxelAtFunc voxelAt) { }

	Voxel voxelAt(ivec3 pos) { return { VEMPTY }; }

	bool voxelAtSafe(ivec3 pos, Voxel& voxel)
	{
		voxel = { VEMPTY };
		return true;
	}

	bool isAVoidChunk() { return true; }

};

