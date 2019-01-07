#ifndef _CHUNK_BUILD_QUEUE_H_
#define _CHUNK_BUILD_QUEUE_H_

#include <stdio.h>
#include <queue>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "MathHelper.h"
#include "Chunk.h"
#include "ChunkSet.h"
#include "ChunkBuilder.h"
#include "DrawableChunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "readerwriterqueue.h"
#include "BecomeActiveObject.h"
#include "CamData.h"

//
// Generate and Mesh
// Gen process involves 3 queues and a map
//
// queue 1: shouldGenerate
// std::queue that's pushed by anyone
// who wants to generate a chunk
//
// queue 2: toGenThreadQ
// thread safe queue for passing chunk positions
// to the gen thread
//
// queue 3: fromGenThreadQ
// thread safe queue for passing generated
// chunk builders to the main thread
//
// the map: holds all chunk builders that have been generated
//
// Mesh process:
//
// TODO: figure out mesh process
// Proposition: any chunk that comes off of gq will be meshed
// (has the benefit of simplicity)
//

typedef std::map<veci3, std::unique_ptr<DrawableChunk>> TMeshedMap;

class ChunkBuildPool
{
public:
	ChunkBuildPool(ChunkSet& _chunkSet, VoxelAtFunc _voxelAt) :
		chunkSet(_chunkSet),
		voxelAt(_voxelAt)
	{
		StartGenThread();
	}

	~ChunkBuildPool() 
	{
		quittingTime = true;
		if(genThread.joinable())
			genThread.join();
	}

#pragma region push-chunks

	void PushShouldGenerate(veci3 cpos)
	{
		shouldGenerate.push(cpos);
	}

	void Generate(int maxGenerateCount);

	void Mesh(int maxChunksToMesh, Material mat, GLuint chunkTriBufferHandle);
	
#pragma endregion

	void Debug()
	{
		printf("meshed has: %d\n", meshed.size());
	}

#pragma region unload

	void Unload(veci3 cpos);

private:
	void Unmesh(veci3 cpos);
	void Ungenerate(veci3 cpos);

public:

#pragma endregion

	// should we be uneasy about this one?
	//std::map<veci3, std::unique_ptr<DrawableChunk>>* getMeshedMap()
	TMeshedMap* getMeshedMap()
	{
		return &meshed;
	}

#pragma region test-draw-drawables

	void TestLoadDrawableBuffers();

	void TestUnloadDrawableBuffers();

	void TestDrawDrawables(const VECam::CamData& cam); // glm::mat4 View, glm::mat4 Proj);

#pragma endregion

	bool debugNull;

private:

	void StartGenThread();
	void GenerateAt(veci3 cpos);

	void Preserve27(veci3 cpos, bool incr);

	moodycamel::BlockingReaderWriterQueue<veci3> toGenThreadQ;
	moodycamel::ReaderWriterQueue< ChunkBuilder *> fromGenThreadQ;
	
	std::queue<veci3> shouldGenerate;
	std::unordered_set<veci3> willGenerate;
	std::unordered_map<veci3, int> preserve;

	TMeshedMap meshed;
	//std::map<veci3, std::unique_ptr<LODDrawableChunkSet>> meshed; // DrawableChunk >> meshed;

	VEActiveObject::BecomeActiveObject active;

	ChunkSet& chunkSet;
	VoxelAtFunc voxelAt;

	std::thread genThread;
	bool isRunnningGenThread;

	bool quittingTime;
};


#endif // !_CHUNK_BUILD_QUEUE_H_

