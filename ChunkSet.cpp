#include "ChunkSet.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace glm;


ChunkSet::~ChunkSet()
{

}

bool ChunkSet::CreateAt(veci3 chunkPos)
{
	if (!WorldContainsChunkPos(chunkPos.toGLMIVec3()))
	{
		return false;
	}
	if (data.find(chunkPos) == data.end())
	{
		//read from disk?
		Chunk* rch = new Chunk(chunkPos.toGLMIVec3(), 0);
		if (iochunk.ReadConstructChunk(*rch))
		{
			std::cout << "storing rch " << chunkPos << std::endl;
			rch->Debug();
			data.insert(std::make_pair(chunkPos, std::unique_ptr<Chunk>(rch)));
			std::cout << "****************\n\n" << std::endl;
		}
		else
		{

			delete rch;
			data.insert(std::make_pair(chunkPos, std::unique_ptr<Chunk>(new Chunk(chunkPos.toGLMIVec3()))));
		}
		//data.insert(std::make_pair(chunkPos, std::unique_ptr<Chunk>(new Chunk(chunkPos.toGLMIVec3()))));
		return true;
	}

	return false;
}

Chunk* ChunkSet::GetOrCreate(veci3 chunkPos)
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

bool ChunkSet::EraseAt(veci3 cpos)
{
	if (data.find(cpos) == data.end())
	{
		return false;
	}

	data.erase(cpos);

	return true;
}

void ChunkSet::Write(veci3 cpos)
{
	auto chunk = Get(cpos);
	if (chunk == nullptr)
	{
		return;
	}

	iochunk.WriteChunk(*chunk);
}
