#pragma once
#include <filesystem>
#include "Chunk.h"
#include "DrawableChunk.h"
#include "MathHelper.h"
#include <string>

class IOChunk
{
	std::filesystem::path bpath;
	std::filesystem::path PathFor(veci3 cpos, bool drawable);
	std::string worldName;

public:
	IOChunk();
	IOChunk(std::string _worldName);
	~IOChunk();

	void Test();

	std::string GetWorldName() { return worldName; }
	void WriteDrawableChunk(DrawableChunk& dc, veci3 cpos);
	void WriteChunk(Chunk & chunk);
	bool ReadConstructChunk(Chunk & chunk);

	void DebugDestroyWorldData();
};

