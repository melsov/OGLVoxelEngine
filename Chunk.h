#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "FlatPow2Array3D.h"
#include "WorldConfig.h"
#include "MeshData.h"
#include "AACube.h"
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
	VTREE_TRUNK,
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
		isMeshDirty(true) 
	{
		Init();
	}

	Chunk(ivec3 _chunkPos, int size_) :
		data(FlatPow2Array3D<Voxel>(size_)),
		chunkPos(_chunkPos),
		pos(NChunkRelative::ChunkPosToPos(_chunkPos)),
		isMeshDirty(true)
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
	

	~Chunk();
	

	int dbugName;


	ChunkMesh::MeshData meshData;

	ChunkMesh::MeshData* getMeshData(int lod = 0) 
	{
		return &meshData;
	}

	//Positions in global coords
	ivec3 position() const { return pos; }
	vec3 positionV3() const { return vec3(pos.x, pos.y, pos.z); }
	vec3 centerV3() const { return positionV3() + VEC3_CHUNK_SIZE / 2.0f; }

	VEMath::AACube& GetAACube() { return aacube; }

	mat4 getModelMatrix()
	{
		return glm::translate(mat4(1.0f), positionV3());
	}
	ivec3 chunkPosition() { return chunkPos; }

	virtual bool getIsEmpty() const { return empty; }
	virtual void updateIsEmpty();
	virtual bool getIsMeshDirty() const { return isMeshDirty; }
	void setIsMeshDirty(bool _dirty) { isMeshDirty = _dirty; }

	virtual void generateVoxels(VoxelAtFunc voxelAt);
	virtual Voxel voxelAt(ivec3 pos);
	virtual bool voxelAtSafe(ivec3 pos, Voxel& voxel);
	virtual bool voxelAtSafe(int x, int y, int z, Voxel& voxel);
	virtual bool isAVoidChunk() { return false; }

	void Debug();

	Voxel* getData() { return data.data(); }

	// used to read data from disk
	void TakeVoxelArray(Voxel* _storage)
	{
		data.TakePointer(_storage, LOG_TWO_CHUNK_SIZE);
	}

	const FlatPow2Array3D<Voxel>& GetFlatArray() { return data; }

	int getSize() const { return data.size(); }

	int getSizeBytes() const { return data.size() * sizeof(Voxel); }

	void TestSetAVoxel(int i, Voxel vox)
	{
		data.get(i) = vox;
	}

	void SetBuildStage(int stage);

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
		aacube = VEMath::AACube(positionV3(), CHUNK_SIZE);
	}

	void doGenerateVoxels(VoxelAtFunc voxelAt);
	Voxel* readFromDisk();
	 
	FlatPow2Array3D<Voxel> data;
	ivec3 chunkPos;
	ivec3 pos;
	bool empty;
	bool isMeshDirty;
	int buildStage;

	VEMath::AACube aacube;
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

	bool voxelAtSafe(int x, int y, int z, Voxel& voxel)
	{
		voxel = { VEMPTY };
		return true;
	}

	bool isAVoidChunk() { return true; }


};

