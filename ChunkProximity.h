#pragma once

#include "ChunkBuildPool.h"
#include "ProximityList.h"
#include "WorldConfig.h"


namespace VEChunkMgmt
{
	typedef veci3(*GetPlayerGlobalPosFunc)();

	class ChunkProximity
	{
	public:
		ChunkProximity(ChunkBuildPool& _builder, GetPlayerGlobalPosFunc _getPlayerPos) :
			builder(_builder),
			getPlayerPos(_getPlayerPos),
			lastChunkPos(veci3(-9999999)),
			proximity(IV_GENERATE_BOX) {}

		~ChunkProximity() {}


		void CurateChunks();

	private:

		ChunkBuildPool& builder;
		GetPlayerGlobalPosFunc getPlayerPos;
		VEProximity::ProximityList proximity;
		veci3 lastChunkPos;
	};

}

