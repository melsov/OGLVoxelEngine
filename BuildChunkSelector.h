#pragma once
#ifndef _BUILD_CHUNK_SELECTOR_H_
#define  _BUILD_CHUNK_SELECTOR_H_

typedef bool(*ShouldGenChunkFunc)(int, int, int);
typedef bool(*ShouldMeshChunkFunc)(int, int, int);
typedef float(*ChunkDistance)(int, int, int);

class BuildChunkSelector
{
private:
	ShouldGenChunkFunc shouldGen;
	ShouldMeshChunkFunc shouldMesh;

public:
	BuildChunkSelector(ShouldMeshChunkFunc _shouldMesh, ShouldGenChunkFunc _shouldGen):
		shouldMesh(_shouldMesh),
		shouldGen(_shouldGen)
	{

	}

	~BuildChunkSelector()
	{

	}
};

#endif // !_BUILD_CHUNK_SELECTOR_H_

