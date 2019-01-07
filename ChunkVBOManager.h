#ifndef _CHUNK_VBO_MANAGER_H_
#define _CHUNK_VBO_MANAGER_H_

#include <map>
#include <memory>
#include <glm/glm.hpp>
#include "MathHelper.h"
#include "DrawableChunk.h"

//
// There are a fixed number of VBOs (pos and uv) (tri indices are the same for all chunks)
// For reasons (player moves), the VBO data must be updated.
// So swap new chunk data for old (out of range) data
// 
// Keep a map that associates chunk positions with indices of an array of data handles
// i.e. there's a struct BufferHandles { int vertHandle; int uvHandle; } 
// and a mem var array BufferHandles[FIXED_BUFFER_COUNT]

// wait...chunk data is not always that same size...
// just (naively?, delete, replace VBOs as needed) ? or maybe the
// apple ogles way allows for differing sizes

// When needed (when player crosses a chunk boundary) 
// 

using namespace glm;

class ChunkVBOManager
{
public:
	ChunkVBOManager();
	~ChunkVBOManager();

private:
	std::map<veci3, DrawableChunk> buffer;
};

#endif // !_CHUNK_VBO_MANAGER_H_
