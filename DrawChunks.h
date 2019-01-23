#pragma once
#include "ChunkBuildPool.h"
#include "CamData.h"
#include "ChunkSet.h"
#include "WorldConfig.h"

class DrawChunks
{

	//TODO: readonly version of map?
	TMeshedMap* meshed;
	ChunkSet& chunkSet;
	float LodRadii[NUM_LODS];

	int GetLOD(float dist);

	bool InFrustum(VECam::CamData& cam, Chunk& chunk);

public:
	DrawChunks(TMeshedMap *_meshed, ChunkSet& _chunkSet);
	~DrawChunks();

	void SetLOD(float innerRad, float decay = .1f);


	void Draw( VECam::CamData& cam);
};

