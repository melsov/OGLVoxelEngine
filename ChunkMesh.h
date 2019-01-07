#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "WorldConfig.h"
#include "RubiksChunk.h"
#include "MeshData.h"

using namespace std;

//
// Chunk Meshing helper functions
//

namespace ChunkMesh
{

#pragma region static-data
	static const float TextureTileSizeX = 1.0 / 3.0;
	static const float TextureTileSizeY = 1.0 / 3.0;


	struct Face
	{
		GLfloat verts[12];
		GLfloat uvs[8];
		unsigned short tris[6];
	};

	enum FaceDirection
	{
		FACE_DOWN, FACE_UP, FACE_LEFT, FACE_RIGHT, FACE_BACK, FACE_FORWARD
	};

	static const glm::ivec3 FaceDirections[6] = {
		glm::ivec3(0, -1, 0),
		glm::ivec3(0, 1, 0),
		glm::ivec3(-1, 0, 0),
		glm::ivec3(1, 0, 0),
		glm::ivec3(0, 0, -1),
		glm::ivec3(0, 0, 1)
	};

	static const GLfloat down[12] = {
		-1, -1, -1, //0
		 1, -1, -1, //1
		 1, -1,  1, //2
		-1, -1,  1, //3
	};

	static const GLfloat up[12] = {
		-1,  1, -1, //0
		-1,  1,  1, //1
		 1,  1,  1, //2
		 1,  1, -1,  //3
	};

	static const GLfloat left[12] = {
		-1, -1,  1, //3
		-1,  1,  1, //2
		-1,  1, -1, //1
		-1, -1, -1, //0
	};

	static const GLfloat right[12] = {
		 1, -1, -1, //0 //3
		 1,  1, -1, //3 //2
		 1,  1,  1, //2 //1
		 1, -1,  1, //1 //0
	};

	static const GLfloat back[12] = {
		-1, -1, -1,
		-1,  1, -1,
		 1,  1, -1,
		 1, -1, -1
	};

	static const GLfloat forwards[12] = {
		 1, -1,  1, //3
		 1,  1,  1, //2
		-1,  1,  1, //1
		-1, -1,  1, //0
	};


	static const GLfloat uvFace[8] = {
		TextureTileSizeX, TextureTileSizeY, //2
		TextureTileSizeX, 0, //3
		0, 0, //0
		0, TextureTileSizeY, // 1
	};


	static const GLuint triFace[6] = {
		0, 1, 2,
		0, 2, 3
	};

	// static array of tri indices: tris are always the same for each chunk
	// only varying in the number of indices to used.
	static unsigned short chunkTriIndices[PROBABLE_WORST_CASE_FACES_PER_CHUNK * 6];

	static GLuint chunkTriBufferHandle;

#pragma endregion

	///*static*/ void InitChunkTriIndices();


	///*static*/ void LoadChunkTriIndices();

	///*static*/ void UnloadChunkTriIndices();

	///*static*/ vec2 TileOffsetForVoxType(unsigned char voxType, int FaceDir);

	///*static*/ void AddFace(
	//	int FaceDir,
	//	unsigned char voxType,
	//	vec3 relPos,
	//	unsigned short relIndex,
	//	MeshData& meshData,
	//	float scale = 1.0f);


	///*static*/ glm::ivec3 FromDir(int dir);

	///*static*/ void CreateMesh(Cube6::RubiksChunk& rubiks, MeshData& md);

	///*static*/ void CreateMeshAtLOD(Cube6::RubiksChunk& rubiks, MeshData& md, int LOD = 0);




	static void InitChunkTriIndices()
	{
		int	baseIndex = 0;
		int i, j;
		for (i = 0; i < PROBABLE_WORST_CASE_FACES_PER_CHUNK; ++i)
		{
			for (j = 0; j < 6; ++j)
			{
				chunkTriIndices[i * 6 + j] = baseIndex + triFace[j];
			}
			baseIndex += 4;
		}
	}


	static void LoadChunkTriIndices()
	{
		glGenBuffers(1, &chunkTriBufferHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkTriBufferHandle);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			PROBABLE_WORST_CASE_FACES_PER_CHUNK * 6 * sizeof(unsigned short),
			&chunkTriIndices[0],
			GL_STATIC_DRAW);
	}

	static void UnloadChunkTriIndices()
	{
		glDeleteBuffers(1, &chunkTriBufferHandle);
	}

	static vec2 TileOffsetForVoxType(unsigned char voxType, int FaceDir)
	{
		vec2 off;
		switch (voxType)
		{
		case VDIRT:
		default:
			off = vec2(0, 0);
		case VGRASS:
			off = FaceDir == FACE_UP ? vec2(2, 0) : vec2(1, 0);
		}

		return off * vec2(TextureTileSizeX, TextureTileSizeY);
	}

	
	// transfer to mesher
	static void AddFace(
		int FaceDir,
		unsigned char voxType,
		vec3 relPos,
		unsigned short relIndex,
		MeshData& meshData,
		float scale = 1.0f)
	{
		static const GLfloat* verts;
		switch (FaceDir)
		{
		case FACE_DOWN: verts = down; break;
		case FACE_UP: verts = up; break;
		case FACE_LEFT: verts = left; break;
		case FACE_RIGHT: verts = right; break;
		case FACE_BACK: verts = back; break;
		case FACE_FORWARD: verts = forwards; break;
		default:
			break;
		}

		
		auto offset = TileOffsetForVoxType(voxType, FaceDir);
		int i, j;
		for (i = 0; i < 4; ++i)
		{
			VVertex vvert;
			for (j = 0; j < 3; ++j)
			{
				vvert.pos[j] = verts[i * 3 + j] / 2.0f * scale + relPos[j];
			}
			for (j = 0; j < 2; ++j)
			{
				vvert.uvs[j] = (j == 1 ? offset.y : offset.x) + uvFace[i * 2 + j];
			}
			GLfloat color[] = {1, .7, .6, 1};
			for (j = 0; j < 4; ++j)
			{
				vvert.color[j] = color[j];
			}
			meshData.verts.push_back(vvert);
		}

		
		for (int i = 0; i < 6; ++i)
		{
			meshData.tris.push_back(triFace[i] + relIndex);
		}
	}
	


	static glm::ivec3 FromDir(int dir)
	{
		return FaceDirections[dir];
	}

	
	//transfer to Mesher
	static void CreateMesh(Cube6::RubiksChunk& rubiks, MeshData& md)
	{
		ivec3 cursor;
		Voxel vox;
		ivec3 relPos;
		int relIndex = 0;

		Chunk* chunk = rubiks.center();

		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			relPos.x = x;
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				relPos.y = y;
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					relPos.z = z;

					chunk->voxelAtSafe(relPos, vox);

					if (vox.type <= VEMPTY) { continue; }

					int face;
					for (face = FACE_DOWN; face <= FACE_FORWARD; ++face)
					{
						cursor = FromDir(face) + relPos;
						bool shouldAdd = true;

						if (rubiks.voxelAtSafe(cursor, vox))
						{
							shouldAdd = vox.type == VEMPTY;
						}

						if (shouldAdd)
						{
							AddFace(face, vox.type, relPos, relIndex, md);
							relIndex += 4;
						}
					}
				}
			}
		}

	}

	static void CreateMeshAtLOD(Cube6::RubiksChunk& rubiks, MeshData& md, int LOD = 1)
	{
		int mlod = (int)(pow(2, LOD) + .5);
		veci3 lodBox(mlod);
		int lodDims = lodBox.area();

		ivec3 cursor;
		Voxel vox;
		ivec3 relPos;
		int relIndex = 0;

		int lindex;

		Chunk* chunk = rubiks.center();

		for (int x = 0; x < CHUNK_SIZE / mlod; ++x)
		{
			relPos.x = x * mlod;
			for (int y = 0; y < CHUNK_SIZE / mlod; ++y)
			{
				relPos.y = y * mlod;
				for (int z = 0; z < CHUNK_SIZE / mlod; ++z)
				{
					relPos.z = z * mlod;

					bool found = false;
					for (lindex = 0; lindex < lodDims; ++lindex)
					{
						auto nudge = NFlatIndex::FromFlatIndexXYZ(lindex, lodBox);
						//printf("nudge: "); DebugVec3(nudge.toGLMIVec3());
						chunk->voxelAtSafe(relPos + nudge.toGLMIVec3(), vox);
						if (vox.type > VEMPTY)
						{
							found = true;
							break;
						}
					}

					if (!found)
					{
						continue;
					}

					int face;
					for (face = FACE_DOWN; face <= FACE_FORWARD; ++face)
					{
						//
						// look over the 4 voxels (if LOD = 1) adjacent to this face
						// TODO: if inside chunk:
						// add only if all in next cube are empty
						// if outside:
						//  if any are empty , add face geometry.
						auto dir = FromDir(face);

						// IF OUTSIDE CHUNK
						bool isLocal = NChunkRelative::isPosChunkLocal(relPos + (dir * mlod));
						cursor = (dir * (isLocal || veci3(dir).allPositiveOrZero() ? mlod : 1)) + relPos;

						auto mask = veci3(dir).ZeroMask();
						auto sideSlice = isLocal ? lodBox : (lodBox * mask) + veci3(dir).abs();
						int sideSliceArea = sideSlice.area();

						bool shouldAdd = isLocal;

						for (int sI = 0; sI < sideSliceArea; ++sI)
						{
							auto nudge = NFlatIndex::FromFlatIndexXYZ(sI, sideSlice);
							if (rubiks.voxelAtSafe(cursor + nudge.toGLMIVec3(), vox))
							{
								if (isLocal)
								{
									if (vox.type > VEMPTY)
									{
										shouldAdd = false;
										break;
									}
								}
								else
								{
									if (vox.type == VEMPTY)
									{
										shouldAdd = true;
										break;
									}
								}
							}
						}

						if (shouldAdd)
						{
							AddFace(face, vox.type, relPos, relIndex, md, mlod);
							relIndex += 4;
						}
					}
				}
			}
		}

	}


	


	static void CreateMeshFake(Chunk& chunk, MeshData& md)
	{

		// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
		// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
		static const GLfloat g_vertex_buffer_data[] = {
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

		// Two UV coordinatesfor each vertex. They were created with Blender.
		static const GLfloat g_uv_buffer_data[] = {
			0.000059f, 1.0f - 0.000004f,
			0.000103f, 1.0f - 0.336048f,
			0.335973f, 1.0f - 0.335903f,
			1.000023f, 1.0f - 0.000013f,
			0.667979f, 1.0f - 0.335851f,
			0.999958f, 1.0f - 0.336064f,
			0.667979f, 1.0f - 0.335851f,
			0.336024f, 1.0f - 0.671877f,
			0.667969f, 1.0f - 0.671889f,
			1.000023f, 1.0f - 0.000013f,
			0.668104f, 1.0f - 0.000013f,
			0.667979f, 1.0f - 0.335851f,
			0.000059f, 1.0f - 0.000004f,
			0.335973f, 1.0f - 0.335903f,
			0.336098f, 1.0f - 0.000071f,
			0.667979f, 1.0f - 0.335851f,
			0.335973f, 1.0f - 0.335903f,
			0.336024f, 1.0f - 0.671877f,
			1.000004f, 1.0f - 0.671847f,
			0.999958f, 1.0f - 0.336064f,
			0.667979f, 1.0f - 0.335851f,
			0.668104f, 1.0f - 0.000013f,
			0.335973f, 1.0f - 0.335903f,
			0.667979f, 1.0f - 0.335851f,
			0.335973f, 1.0f - 0.335903f,
			0.668104f, 1.0f - 0.000013f,
			0.336098f, 1.0f - 0.000071f,
			0.000103f, 1.0f - 0.336048f,
			0.000004f, 1.0f - 0.671870f,
			0.336024f, 1.0f - 0.671877f,
			0.000103f, 1.0f - 0.336048f,
			0.336024f, 1.0f - 0.671877f,
			0.335973f, 1.0f - 0.335903f,
			0.667969f, 1.0f - 0.671889f,
			1.000004f, 1.0f - 0.671847f,
			0.667979f, 1.0f - 0.335851f
		};


		std::vector<GLfloat> verts(g_vertex_buffer_data, g_vertex_buffer_data + sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]));
		std::vector<GLfloat> uvs(g_uv_buffer_data, g_uv_buffer_data + sizeof(g_uv_buffer_data) / sizeof(g_vertex_buffer_data[0]));

		// very simple (pointless for now) tri indices
		std::vector<unsigned short> indices;
		for (size_t i = 0; i < verts.size(); ++i)
		{
			indices.push_back((unsigned short)i);
		}

		throw 7; // we broke md.verts: now a struct

		//md.verts = verts; // want
		/*md.uvs = uvs;
		md.tris = indices;*/

	}
}

// header version probs?
//
//#pragma once
//#include <vector>
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include "Chunk.h"
//#include "WorldConfig.h"
//#include "RubiksChunk.h"
//#include "MeshData.h"
//
//
//using namespace std;
//
//
//namespace ChunkMesh
//{
//
//#pragma region static-data
//	static const float TextureTileSizeX = 1.0 / 3.0;
//	static const float TextureTileSizeY = 1.0 / 3.0;
//
//
//	struct Face
//	{
//		GLfloat verts[12];
//		GLfloat uvs[8];
//		unsigned short tris[6];
//	};
//
//	enum FaceDirection
//	{
//		FACE_DOWN, FACE_UP, FACE_LEFT, FACE_RIGHT, FACE_BACK, FACE_FORWARD
//	};
//
//	static const glm::ivec3 FaceDirections[6] = {
//		glm::ivec3(0, -1, 0),
//		glm::ivec3(0, 1, 0),
//		glm::ivec3(-1, 0, 0),
//		glm::ivec3(1, 0, 0),
//		glm::ivec3(0, 0, -1),
//		glm::ivec3(0, 0, 1)
//	};
//
//	static const GLfloat down[12] = {
//		-1, -1, -1, //0
//		 1, -1, -1, //1
//		 1, -1,  1, //2
//		-1, -1,  1, //3
//	};
//
//	static const GLfloat up[12] = {
//		-1,  1, -1, //0
//		-1,  1,  1, //1
//		 1,  1,  1, //2
//		 1,  1, -1,  //3
//	};
//
//	static const GLfloat left[12] = {
//		-1, -1,  1, //3
//		-1,  1,  1, //2
//		-1,  1, -1, //1
//		-1, -1, -1, //0
//	};
//
//	static const GLfloat right[12] = {
//		 1, -1, -1, //0 //3
//		 1,  1, -1, //3 //2
//		 1,  1,  1, //2 //1
//		 1, -1,  1, //1 //0
//	};
//
//	static const GLfloat back[12] = {
//		-1, -1, -1,
//		-1,  1, -1,
//		 1,  1, -1,
//		 1, -1, -1
//	};
//
//	static const GLfloat forwards[12] = {
//		 1, -1,  1, //3
//		 1,  1,  1, //2
//		-1,  1,  1, //1
//		-1, -1,  1, //0
//	};
//
//
//	static const GLfloat uvFace[8] = {
//		TextureTileSizeX, TextureTileSizeY, //2
//		TextureTileSizeX, 0, //3
//		0, 0, //0
//		0, TextureTileSizeY, // 1
//	};
//
//
//	static const GLuint triFace[6] = {
//		0, 1, 2,
//		0, 2, 3
//	};
//
//	// static array of tri indices: tris are always the same for each chunk
//	// only varying in the number of indices to used.
//	static unsigned short chunkTriIndices[PROBABLE_WORST_CASE_FACES_PER_CHUNK * 6];
//
//#pragma endregion
//
//	/*static*/ void InitChunkTriIndices();
//
//	static GLuint chunkTriBufferHandle;
//
//	/*static*/ void LoadChunkTriIndices();
//
//	/*static*/ void UnloadChunkTriIndices();
//
//	/*static*/ vec2 TileOffsetForVoxType(unsigned char voxType, int FaceDir);
//
//	/*static*/ void AddFace(
//		int FaceDir,
//		unsigned char voxType,
//		vec3 relPos,
//		unsigned short relIndex,
//		MeshData& meshData,
//		float scale = 1.0f);
//
//
//	/*static*/ glm::ivec3 FromDir(int dir);
//
//	/*static*/ void CreateMesh(Cube6::RubiksChunk& rubiks, MeshData& md);
//
//	/*static*/ void CreateMeshAtLOD(Cube6::RubiksChunk& rubiks, MeshData& md, int LOD = 0);
//
//	static void CreateMeshFake(Chunk& chunk, MeshData& md)
//	{
//
//		// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
//		// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
//		static const GLfloat g_vertex_buffer_data[] = {
//			-1.0f,-1.0f,-1.0f,
//			-1.0f,-1.0f, 1.0f,
//			-1.0f, 1.0f, 1.0f,
//			1.0f, 1.0f,-1.0f,
//			-1.0f,-1.0f,-1.0f,
//			-1.0f, 1.0f,-1.0f,
//			1.0f,-1.0f, 1.0f,
//			-1.0f,-1.0f,-1.0f,
//			1.0f,-1.0f,-1.0f,
//			1.0f, 1.0f,-1.0f,
//			1.0f,-1.0f,-1.0f,
//			-1.0f,-1.0f,-1.0f,
//			-1.0f,-1.0f,-1.0f,
//			-1.0f, 1.0f, 1.0f,
//			-1.0f, 1.0f,-1.0f,
//			1.0f,-1.0f, 1.0f,
//			-1.0f,-1.0f, 1.0f,
//			-1.0f,-1.0f,-1.0f,
//			-1.0f, 1.0f, 1.0f,
//			-1.0f,-1.0f, 1.0f,
//			1.0f,-1.0f, 1.0f,
//			1.0f, 1.0f, 1.0f,
//			1.0f,-1.0f,-1.0f,
//			1.0f, 1.0f,-1.0f,
//			1.0f,-1.0f,-1.0f,
//			1.0f, 1.0f, 1.0f,
//			1.0f,-1.0f, 1.0f,
//			1.0f, 1.0f, 1.0f,
//			1.0f, 1.0f,-1.0f,
//			-1.0f, 1.0f,-1.0f,
//			1.0f, 1.0f, 1.0f,
//			-1.0f, 1.0f,-1.0f,
//			-1.0f, 1.0f, 1.0f,
//			1.0f, 1.0f, 1.0f,
//			-1.0f, 1.0f, 1.0f,
//			1.0f,-1.0f, 1.0f
//		};
//
//		// Two UV coordinatesfor each vertex. They were created with Blender.
//		static const GLfloat g_uv_buffer_data[] = {
//			0.000059f, 1.0f - 0.000004f,
//			0.000103f, 1.0f - 0.336048f,
//			0.335973f, 1.0f - 0.335903f,
//			1.000023f, 1.0f - 0.000013f,
//			0.667979f, 1.0f - 0.335851f,
//			0.999958f, 1.0f - 0.336064f,
//			0.667979f, 1.0f - 0.335851f,
//			0.336024f, 1.0f - 0.671877f,
//			0.667969f, 1.0f - 0.671889f,
//			1.000023f, 1.0f - 0.000013f,
//			0.668104f, 1.0f - 0.000013f,
//			0.667979f, 1.0f - 0.335851f,
//			0.000059f, 1.0f - 0.000004f,
//			0.335973f, 1.0f - 0.335903f,
//			0.336098f, 1.0f - 0.000071f,
//			0.667979f, 1.0f - 0.335851f,
//			0.335973f, 1.0f - 0.335903f,
//			0.336024f, 1.0f - 0.671877f,
//			1.000004f, 1.0f - 0.671847f,
//			0.999958f, 1.0f - 0.336064f,
//			0.667979f, 1.0f - 0.335851f,
//			0.668104f, 1.0f - 0.000013f,
//			0.335973f, 1.0f - 0.335903f,
//			0.667979f, 1.0f - 0.335851f,
//			0.335973f, 1.0f - 0.335903f,
//			0.668104f, 1.0f - 0.000013f,
//			0.336098f, 1.0f - 0.000071f,
//			0.000103f, 1.0f - 0.336048f,
//			0.000004f, 1.0f - 0.671870f,
//			0.336024f, 1.0f - 0.671877f,
//			0.000103f, 1.0f - 0.336048f,
//			0.336024f, 1.0f - 0.671877f,
//			0.335973f, 1.0f - 0.335903f,
//			0.667969f, 1.0f - 0.671889f,
//			1.000004f, 1.0f - 0.671847f,
//			0.667979f, 1.0f - 0.335851f
//		};
//
//
//		std::vector<GLfloat> verts(g_vertex_buffer_data, g_vertex_buffer_data + sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]));
//		std::vector<GLfloat> uvs(g_uv_buffer_data, g_uv_buffer_data + sizeof(g_uv_buffer_data) / sizeof(g_vertex_buffer_data[0]));
//
//		// very simple (pointless for now) tri indices
//		std::vector<unsigned short> indices;
//		for (size_t i = 0; i < verts.size(); ++i)
//		{
//			indices.push_back((unsigned short)i);
//		}
//
//		throw 7; // we broke md.verts: now a struct
//
//		//md.verts = verts; // want
//		md.uvs = uvs;
//		md.tris = indices;
//
//	}
//}