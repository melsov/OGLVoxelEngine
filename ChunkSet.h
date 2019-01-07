#pragma once
#include <map>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "MathHelper.h"
#include <vector>
#include <memory>
#include "WorldConfig.h"
#include <Windows.h>

class ChunkSet
{
public:
	ChunkSet() :
		_voidChunk(ivec3(-9999, -9999, -9999))
	{
	}

	~ChunkSet() 
	{
	}

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

	Chunk* GetOrCreate(veci3 chunkPos)
	{
		bool debugCheck = CreateAt(chunkPos);

		// DEBUG
		if (debugCheck)
		{
			auto ch = Get(chunkPos);
			if (ch == nullptr)
			{
				printf("chunk set *** ! we're storing a null chunk ptr at: "); DebugVec3(chunkPos.toGLMIVec3());
			}
		}
		else
		{
			if (WorldContainsChunkPos(chunkPos.toGLMIVec3()))
			{
				auto ch = Get(chunkPos);
				if (ch == nullptr)
				{
					printf("chunk set *** ! we're storing a null chunk ptr at: "); DebugVec3(chunkPos.toGLMIVec3());
				}
			}
		}
		//END DEBUG

		return Get(chunkPos);
	}


	bool CreateAt(veci3 chunkPos)
	{
		if (!WorldContainsChunkPos(chunkPos.toGLMIVec3()))
		{
			return false;
		}
		if (data.find(chunkPos) == data.end())// !Contains(chunkPos)) 
		{
			data.insert(std::make_pair(chunkPos, std::unique_ptr<Chunk>(new Chunk(chunkPos.toGLMIVec3()))));
			return true;
		}

		return false;
	}

	bool EraseAt(veci3 cpos)
	{
		if (data.find(cpos) == data.end())
		{
			return false;
		}

		data.erase(cpos);

		return true;
	}

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

