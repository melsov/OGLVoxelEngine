#include "ChunkBuildPool.h"
#include <chrono>
#include <iostream>
#include "CamData.h"
#include "WorldConfig.h"
#include "MathHelper.h"

#pragma region push-chunks


// TODO: plan, chunk build pool builds in columns
// 

void ChunkBuildPool::Generate(int maxGenerateCount)
{
	while (!shouldGenerate.empty())
	{
		auto cpos = shouldGenerate.front();
		shouldGenerate.pop();

		// already generating?
		if (willGenerate.find(cpos) != willGenerate.end()) { continue; }
		
		auto chunk = chunkSet.GetOrCreate(cpos);

		if (chunkSet.DebugContainsButIsNullOrVoid(cpos)) { 
			printf("ChBPool nullptr at: "); DebugVec3(cpos.toGLMIVec3()); continue; 
		}

		willGenerate.insert(cpos); 
		Preserve27(cpos, true);
		toGenThreadQ.enqueue(cpos);

		if (--maxGenerateCount <= 0) { break; }
	}
	 
}


void ChunkBuildPool::Preserve27(veci3 cpos, bool incr)
{
	for (int i = 0; i < 27; ++i)
	{
		auto subj = veci3(IDirections::xyz[i]) + cpos;
		int count = 0;
		auto it = preserve.find(subj);
		if (it != preserve.end())
		{
			count = it->second;
		}
		preserve[subj] = incr ? ++count : --count;
	}
}

#pragma region gen-thread

void ChunkBuildPool::StartGenThread()
{
	if (isRunnningGenThread) { return; }
	isRunnningGenThread = true;

	// [&]  means capture vars in scope by reference
	// needed to capture 'this'
	genThread = std::thread([&]()
	{
		veci3 cpos;
		while (!quittingTime)
		{
			if (!toGenThreadQ.wait_dequeue_timed(cpos, std::chrono::milliseconds(200)))
			{
				continue;
			}

			GenerateAt(cpos);
		}
	});
}

//
//Build on the gen thread
//
void ChunkBuildPool::GenerateAt(veci3 cpos)
{
	auto builder = new ChunkBuilder(cpos.toGLMIVec3());
	builder->Generate(voxelAt, chunkSet);
	fromGenThreadQ.enqueue(builder);
}

#pragma endregion



//
// pull positions from fromGenThhreadQ
// until empty or we've hit maxChunksToMesh
//
void ChunkBuildPool::Mesh(int maxChunksToMesh, Material mat, GLuint chunkTriBufferHandle)
{
	ChunkBuilder* builder;
	while (fromGenThreadQ.try_dequeue(builder))
	{

		auto cpos = veci3(builder->GetCenterChunkPos());

		if (meshed.find(cpos) != meshed.end()) 
		{
			printf("already meshed at: "); DebugVec3(cpos.toGLMIVec3());
			continue; 
		} // or is the chunk dirty?

		auto chunk = chunkSet.Get(builder->GetCenterChunkPos());
		
		builder->Mesh(chunk->meshData); 

		//
		// Troubles?
		//
		//LODDrawableChunkSet* lodSet = new LODDrawableChunkSet();
		//for (int i = 0; i < NUM_LODS; ++i)
		//{
		//	lodSet->set[i] = new DrawableChunk(chunk->getModelMatrix(), mat, chunkTriBufferHandle);
		//	lodSet->set[i]->md = chunk->getMeshData(i);
		//	lodSet->set[i]->LoadBuffers();

		//	//DrawableChunk* drawable = new DrawableChunk(chunk->getModelMatrix(), mat, chunkTriBufferHandle);
		//	//drawable->md = chunk->getMeshData();
		//	//drawable->LoadBuffers();
		//}
		//meshed.insert(std::make_pair(cpos, lodSet)); // drawable));


		DrawableChunk* drawable = new DrawableChunk(
			chunk->getModelMatrix(), 
			mat, 
			chunkTriBufferHandle); 
		drawable->md = &chunk->meshData; // &chunk->meshSet;
		drawable->LoadBuffers();
		meshed.insert(std::make_pair(cpos, drawable));

		Preserve27(cpos, false);

		delete builder;

		if (--maxChunksToMesh <= 0) { 
			break; 
		}
	}
}

// TODO: mechanism for re-meshing
// must erase from willGenerate
// and from meshed?
// or (BETTER) just create an alternate pipeline for immediate remeshing
// replace the drawable in meshed map

#pragma endregion

#pragma region unload

void ChunkBuildPool::Unload(veci3 cpos)
{
	auto it = preserve.find(cpos);
	if (it != preserve.end())
	{
		if (it->second > 0)
		{
			if (debugNull)
			{
				std::cout << " preserve " << it->second << std::endl;
			}
			return; //still preserving
		}

		preserve.erase(cpos);
	}
	Unmesh(cpos);
	Ungenerate(cpos);
}

void ChunkBuildPool::Unmesh(veci3 cpos)
{
	if (meshed.find(cpos) == meshed.end())
	{
		return;
	}
	auto drawable = meshed.find(cpos)->second.get();
	//CONSIDER: is this safe?
	glBindBuffer(GL_ARRAY_BUFFER, 0); // bind to nothing so that the deletes actually happen?
	drawable->UnloadBuffers();
	
	meshed.erase(cpos);
}

// TODO: this allows chunks that might be 
// in the generate q to be erased...
// devise a way to coordinate erasing with 
// the needs of the gen thread and also meshing
void ChunkBuildPool::Ungenerate(veci3 cpos)
{
	if (willGenerate.find(cpos) == willGenerate.end())
	{
		return;
	}

	willGenerate.erase(cpos);
	chunkSet.EraseAt(cpos);
}




#pragma endregion

#pragma region test-draw-drawables

void ChunkBuildPool::TestLoadDrawableBuffers()
{
	//printf("CBPool testLoad buffs meshed size %d \n", meshed.size());
	//for (auto it = meshed.begin(); it != meshed.end(); ++it)
	//{
	//	//printf("loading a drawabbble's buffer "); DebugVec3(it->first.toGLMIVec3());
	//	it->second.get()->LoadBuffers();
	//}
}

void ChunkBuildPool::TestUnloadDrawableBuffers()
{
	for (auto it = meshed.begin(); it != meshed.end(); ++it)
	{
		it->second.get()->UnloadBuffers();
	}
}

void ChunkBuildPool::TestDrawDrawables(const VECam::CamData& cam) 
{
	auto View = cam.getViewMatrix();
	auto Proj = cam.getProjectionMatrix();

	for (auto it = meshed.begin(); it != meshed.end(); ++it)
	{
		auto chunk = chunkSet.Get(it->first);
		if (chunk == nullptr || chunk->getIsEmpty())
		{
			continue;
		}

		bool inFrustum = false;
		auto cpos = chunk->positionV3();
		for (int i = 0; i < 8; ++i)
		{
			auto p = cpos + (IDirections::EightCorners[i] * (float)CHUNK_SIZE);
			if (cam.IsPointInFrustum(p))
			{
				inFrustum = true;
				break;
			}
		}

		if (inFrustum)
		{
			int lod = 0; 
			// TODO: choose lod func. based on cpos cam pos;
			// it->second.get()->set[lod]->Draw(View, Proj);
			it->second.get()->Draw(View, Proj);
		}

	}

}


#pragma endregion