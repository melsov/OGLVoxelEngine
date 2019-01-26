#pragma once
#include <map>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "MathHelper.h"
#include <vector>
#include <memory>
#include "WorldConfig.h"
#include <Windows.h>
#include <string>
#include "IOChunk.h"

class ChunkSet
{
	IOChunk iochunk;
public:
	ChunkSet() : ChunkSet(DEFAULT_WORLD_NAME) {}

	ChunkSet(std::string _worldName) :
		iochunk(_worldName),
		_voidChunk(ivec3(-9999, -9999, -9999))
	{
		iochunk.DebugDestroyWorldData(); //DBUG
	}

	~ChunkSet();
	
	IOChunk& GetIOChunk() { return iochunk; }

	std::string GetWorldName() { return iochunk.GetWorldName(); }

	bool Contains(veci3 chunkPos) const
	{
		return data.find(chunkPos) != data.end(); 
	}

	Chunk* Get(veci3 chunkPos) 
	{
		if (!WorldContainsChunkPos(chunkPos.toGLMIVec3()))
		{
			return &_voidChunk;
		}
		if (Contains(chunkPos)) 
		{
			return data[chunkPos].get();
		}
		return nullptr;
	}


	bool GetSafe(veci3 chunkPos, Chunk** chunk) 
	{
		*chunk = Get(chunkPos);
		return *chunk != nullptr;
	}

	bool DebugContainsButIsNullOrVoid(veci3 cpos)
	{
		if (Contains(cpos))
		{
			auto ch = Get(cpos);
			return ch == nullptr || ch->isAVoidChunk();
		}
		return false;
	}

	bool CreateAt(veci3 chunkPos);

	Chunk* GetOrCreate(veci3 chunkPos);

	void Write(veci3 cpos);

	bool EraseAt(veci3 cpos);
	


	void DebugContents()
	{
		printf("*ChunkSet Contents: \n");
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			printf("CH: %s ", it->second.get() == nullptr ? "NULL" : "GOOD"); DebugVec3(it->first.toGLMIVec3());
		}
		printf("END CSContents *\n\n");
	}

private:
	std::map<veci3, std::unique_ptr<Chunk>> data;
	VoidChunk _voidChunk;
};

