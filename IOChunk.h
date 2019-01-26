#pragma once
#include <filesystem>
#include "Chunk.h"
#include "DrawableChunk.h"
#include "MathHelper.h"
#include <string>
#include "MeshData.h"

class IOChunk
{
	//std::filesystem::path bpath;
	std::string bpath;
	//std::filesystem::path PathFor(veci3 cpos, bool drawable);
	std::string PathFor(veci3 cpos, bool drawable);
	std::string worldName;

	bool ReadTriOffsets(ChunkMesh::MeshData & md, veci3 cpos);
	bool ReadTris(ChunkMesh::MeshData & md, veci3 cpos);
	bool ReadVerts(ChunkMesh::MeshData & md, veci3 cpos);

public:
	IOChunk();
	IOChunk(std::string _worldName);
	~IOChunk();

	void Test();

	std::string GetWorldName() { return worldName; }
	void WriteDrawableChunk(ChunkMesh::MeshData & md, veci3 cpos);
	//void WriteDrawableChunk(DrawableChunk& dc, veci3 cpos);
	bool ReadConstructDrawableChunk(ChunkMesh::MeshData& md, veci3 cpos);
	//bool ReadConstructDrawableChunk(DrawableChunk& dc, veci3 cpos);
	void WriteChunk(Chunk & chunk);
	bool ReadConstructChunk(Chunk & chunk);



	void DebugDestroyWorldData();
};

