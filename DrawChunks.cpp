#include "DrawChunks.h"



DrawChunks::DrawChunks(TMeshedMap * _meshed, ChunkSet& _chunkSet) :
	meshed(_meshed),
	chunkSet(_chunkSet)
{
}

DrawChunks::~DrawChunks()
{
}

void DrawChunks::SetLOD(float innerRad, float decay)
{
	for (int i = 0; i < NUM_LODS; ++i)
	{
		LodRadii[i] = innerRad;
		innerRad += innerRad * decay;
	}
}

int DrawChunks::GetLOD(float dist)
{
	for (int i = 0; i < NUM_LODS; ++i)
	{
		if (dist < LodRadii[i]) return i;
	}
	return NUM_LODS - 1;
}

bool DrawChunks::InFrustum(VECam::CamData& cam, Chunk& chunk)
{

	auto cpos = chunk.positionV3();
	float closestForwardDistSq = 999999999.0f;
	for (int i = 0; i < 8; ++i)
	{
		auto p = cpos + (IDirections::EightCorners[i] * (float)CHUNK_SIZE);
		if (cam.IsPointInFrustum(p))
		{
			return true;
		}

		//fallback calculation
		auto dif = (p - cam.getPosition());
		float dist = glm::dot(dif, dif);
		if (dist < closestForwardDistSq && glm::dot(dif, cam.getDirection()) > 0.0f)
		{
			closestForwardDistSq = dist;
		}
	}

	// we might be quite close to the chunk
	// unable to see any of its corners
	return (closestForwardDistSq < (CHUNK_SIZE * CHUNK_SIZE));

	
	// Nice to use math, but this doesn't work. (Plus, it should be slower)
	//return cam.IsCubeInFrustum(chunk.GetAACube());
}



void DrawChunks::Draw( VECam::CamData& cam)
{
	auto View = cam.getViewMatrix();
	auto Proj = cam.getProjectionMatrix();

	for (auto it = meshed->begin(); it != meshed->end(); ++it)
	{
		auto chunk = chunkSet.Get(it->first);
		if (chunk == nullptr || chunk->getIsEmpty())
		{
			continue;
		}

		auto cpos = chunk->positionV3();
		if (InFrustum(cam, *chunk))
		{
			// find least distance
			glm::float32 leastDistance = 99999999.0f;
			for (int i = 0; i < 8; ++i)
			{
				auto p = cpos + (IDirections::EightCorners[i] * (float)CHUNK_SIZE) - cam.getPosition();
				float distSq = glm::dot(p, p);
				if (distSq < leastDistance) { leastDistance = distSq; }
			}
			leastDistance = glm::sqrt(leastDistance);

			// find lod with distance from cam
			int lod = GetLOD(leastDistance);
			it->second.get()->Draw(View, Proj, lod);
		}

	}

}