#include "ChunkProximity.h"

namespace VEChunkMgmt
{


	void ChunkProximity::CurateChunks()
	{
		//
		// Load
		//
		auto pos = getPlayerPos();
		veci3 relPos;
		veci3 cpos = NChunkRelative::PosToChunkPos(pos, relPos);
		if (cpos == lastChunkPos)
		{
			return;
		}

		bool teleported = !(lastChunkPos - cpos).abs().allLessThan(veci3(2));
		
		veci3 subject;

		proximity.reset(teleported);
		while (proximity.next(teleported, subject))
		{
			if (!WorldContainsChunkPos(subject + cpos)) { continue; }

			builder.PushShouldGenerate(subject + cpos);
		}

		//
		// Unload
		//
		auto meshed = builder.getMeshedMap(); 
		vector<veci3> unmesh;

		// find out-of-range positions
		for (auto it = meshed->begin(); it != meshed->end(); ++it)
		{
			veci3 rel;
			veci3 pcpos = NChunkRelative::PosToChunkPos(getPlayerPos(), rel);
			auto dif = (it->first - cpos).abs();

			auto halfPreserve = IV_PRESERVE_CHUNK_BOX / 2;
			if (!dif.allLessOrEqual(halfPreserve))
			{
				unmesh.push_back(it->first);
			}
		}

		// unload out-of-range positions
		for (auto it = unmesh.begin(); it != unmesh.end(); ++it)
		{
			builder.Unload(*it);
		}

		
	}

}