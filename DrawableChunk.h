#pragma once

#include "Material.h"
#include "MeshData.h"
#include <vector>
#include <memory>



typedef std::vector<GLfloat> TStorFloat;

class DrawableChunk 
{
public:
	DrawableChunk(glm::mat4 _ModelMatrix, Material _mat, GLuint _trisBufferHandle) :
		ModelMatrix(_ModelMatrix),
		mat(_mat)
	{
#ifdef STATIC_TRI_INDICES
		handles.tris = _trisBufferHandle;
#endif // STATIC_TRI_INDICES

	}

	~DrawableChunk() 
	{
		printf("DrawableChunk DTOR\n");
	}

	void Debug();
	void UnloadBuffers();
	void LoadBuffers();
	void Draw(glm::mat4 View, glm::mat4 Projection, int LOD = 0);

	//BufferHandles GetBufferHandles() { return handles; }

	//ChunkMesh::LODMeshDataSet* md;
	ChunkMesh::MeshData* md;

	//std::vector<GLfloat>* verts;
	//std::vector<GLfloat>* uvs;

	Material mat;

	glm::mat4 ModelMatrix;



private:

	auto verts(int lod = 0)
	{
		return &md->verts;
		//TODO: per lod
		//return &(md->mds[lod].verts);
	}

	//TStorFloat* uvs(int lod = 0)
	//{
	//	//return &md->mds[lod].uvs;
	//	return &md->uvs;
	//}

	ChunkMesh::BufferHandles handles;
	bool loaded;

};

class LODDrawableChunkSet
{
public:
	LODDrawableChunkSet() {}
	~LODDrawableChunkSet()
	{
		delete[] set;
	}

	//std::unique_ptr<DrawableChunk> set[NUM_LODS];
	//std::vector<std::unique_ptr<DrawableChunk>> set;
	DrawableChunk* set[NUM_LODS];

	void UnloadBuffers()
	{
		for (int i = 0; i < NUM_LODS; ++i)
		{
			set[i]->UnloadBuffers();
			//set[i].get()->UnloadBuffers();
		}
	}
};



