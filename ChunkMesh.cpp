#include "ChunkMesh.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "WorldConfig.h"
#include "RubiksChunk.h"
#include "MeshData.h"


using namespace std;


namespace ChunkMesh
{
	//
	///*static*/ void InitChunkTriIndices()
	//{
	//	int	baseIndex = 0;
	//	int i, j;
	//	for (i = 0; i < PROBABLE_WORST_CASE_FACES_PER_CHUNK; ++i)
	//	{
	//		for (j = 0; j < 6; ++j)
	//		{
	//			chunkTriIndices[i * 6 + j] = baseIndex + triFace[j];
	//		}
	//		baseIndex += 4;
	//	}
	//}


	///*static*/ void LoadChunkTriIndices()
	//{
	//	glGenBuffers(1, &chunkTriBufferHandle);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkTriBufferHandle);
	//	glBufferData(
	//		GL_ELEMENT_ARRAY_BUFFER,
	//		PROBABLE_WORST_CASE_FACES_PER_CHUNK * 6 * sizeof(unsigned short),
	//		&chunkTriIndices[0],
	//		GL_STATIC_DRAW);
	//}

	///*static*/ void UnloadChunkTriIndices()
	//{
	//	glDeleteBuffers(1, &chunkTriBufferHandle);
	//}

	///*static*/ vec2 TileOffsetForVoxType(unsigned char voxType, int FaceDir)
	//{
	//	vec2 off;
	//	switch (voxType)
	//	{
	//	case VDIRT:
	//	default:
	//		off = vec2(0, 0);
	//	case VGRASS:
	//		off = FaceDir == FACE_UP ? vec2(2, 0) : vec2(1, 0);
	//	}

	//	return off * vec2(TextureTileSizeX, TextureTileSizeY);
	//}

	///*static*/ void AddFace(
	//	int FaceDir,
	//	unsigned char voxType,
	//	vec3 relPos,
	//	unsigned short relIndex,
	//	MeshData& meshData,
	//	float scale)
	//{
	//	static const GLfloat* verts;
	//	switch (FaceDir)
	//	{
	//	case FACE_DOWN: verts = down; break;
	//	case FACE_UP: verts = up; break;
	//	case FACE_LEFT: verts = left; break;
	//	case FACE_RIGHT: verts = right; break;
	//	case FACE_BACK: verts = back; break;
	//	case FACE_FORWARD: verts = forwards; break;
	//	default:
	//		break;
	//	}

	//	/*for (int i = 0; i < 12; ++i)
	//	{
	//		meshData.verts.push_back((GLfloat)(*(verts + i) / 2.0 * scale + relPos[i % 3]));
	//	}*/
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		VVertex vvert;
	//		for (int j = 0; j < 3; ++j)
	//		{
	//			vvert.pos[j] = verts[i * 3 + j] / 2.0f * scale + relPos[j];
	//		}
	//		meshData.verts.push_back(vvert);
	//	}

	//	auto offset = TileOffsetForVoxType(voxType, FaceDir);
	//	for (int i = 0; i < 8; ++i)
	//	{
	//		GLfloat off = (i & 1) == 0 ? offset.x : offset.y;
	//		meshData.uvs.push_back(off + uvFace[i]);
	//	}
	//	for (int i = 0; i < 6; ++i)
	//	{
	//		meshData.tris.push_back(triFace[i] + relIndex);
	//	}
	//}



	///*static*/ glm::ivec3 FromDir(int dir)
	//{
	//	return FaceDirections[dir];
	//}



	///*static*/ void CreateMesh(Cube6::RubiksChunk& rubiks, MeshData& md)
	//{
	//	ivec3 cursor;
	//	Voxel vox;
	//	ivec3 relPos;
	//	int relIndex = 0;

	//	Chunk* chunk = rubiks.center();

	//	for (int x = 0; x < CHUNK_SIZE; ++x)
	//	{
	//		relPos.x = x;
	//		for (int y = 0; y < CHUNK_SIZE; ++y)
	//		{
	//			relPos.y = y;
	//			for (int z = 0; z < CHUNK_SIZE; ++z)
	//			{
	//				relPos.z = z;

	//				chunk->voxelAtSafe(relPos, vox);

	//				if (vox.type <= VEMPTY) { continue; }

	//				int face;
	//				for (face = FACE_DOWN; face <= FACE_FORWARD; ++face)
	//				{
	//					cursor = FromDir(face) + relPos;
	//					bool shouldAdd = true;

	//					if (rubiks.voxelAtSafe(cursor, vox))
	//					{
	//						shouldAdd = vox.type == VEMPTY;
	//					}

	//					if (shouldAdd)
	//					{
	//						AddFace(face, vox.type, relPos, relIndex, md);
	//						relIndex += 4;
	//					}
	//				}
	//			}
	//		}
	//	}

	//}

	///*static*/ void CreateMeshAtLOD(Cube6::RubiksChunk& rubiks, MeshData& md, int LOD)
	//{
	//	int mlod = (int)(pow(2, LOD) + .5);
	//	veci3 lodBox(mlod);
	//	int lodDims = lodBox.area();

	//	ivec3 cursor;
	//	Voxel vox;
	//	ivec3 relPos;
	//	int relIndex = 0;

	//	int lindex;

	//	Chunk* chunk = rubiks.center();

	//	for (int x = 0; x < CHUNK_SIZE / mlod; ++x)
	//	{
	//		relPos.x = x * mlod;
	//		for (int y = 0; y < CHUNK_SIZE / mlod; ++y)
	//		{
	//			relPos.y = y * mlod;
	//			for (int z = 0; z < CHUNK_SIZE / mlod; ++z)
	//			{
	//				relPos.z = z * mlod;

	//				bool found = false;
	//				for (lindex = 0; lindex < lodDims; ++lindex)
	//				{
	//					auto nudge = NFlatIndex::FromFlatIndexXYZ(lindex, lodBox);
	//					//printf("nudge: "); DebugVec3(nudge.toGLMIVec3());
	//					chunk->voxelAtSafe(relPos + nudge.toGLMIVec3(), vox);
	//					if (vox.type > VEMPTY)
	//					{
	//						found = true;
	//						break;
	//					}
	//				}

	//				if (!found)
	//				{
	//					continue;
	//				}

	//				int face;
	//				for (face = FACE_DOWN; face <= FACE_FORWARD; ++face)
	//				{
	//					//
	//					// look over the 4 voxels (if LOD = 1) adjacent to this face
	//					// TODO: if inside chunk:
	//					// add only if all in next cube are empty
	//					// if outside:
	//					//  if any are empty , add face geometry.
	//					auto dir = FromDir(face);

	//					// IF OUTSIDE CHUNK
	//					bool isLocal = NChunkRelative::isPosChunkLocal(relPos + (dir * mlod));
	//					cursor = (dir * (isLocal || veci3(dir).allPositiveOrZero() ? mlod : 1)) + relPos;

	//					auto mask = veci3(dir).ZeroMask();
	//					auto sideSlice = isLocal ? lodBox : (lodBox * mask) + veci3(dir).abs();
	//					int sideSliceArea = sideSlice.area();

	//					bool shouldAdd = isLocal;

	//					for (int sI = 0; sI < sideSliceArea; ++sI)
	//					{
	//						auto nudge = NFlatIndex::FromFlatIndexXYZ(sI, sideSlice);
	//						if (rubiks.voxelAtSafe(cursor + nudge.toGLMIVec3(), vox))
	//						{
	//							if (isLocal)
	//							{
	//								if (vox.type > VEMPTY)
	//								{
	//									shouldAdd = false;
	//									break;
	//								}
	//							}
	//							else
	//							{
	//								if (vox.type == VEMPTY)
	//								{
	//									shouldAdd = true;
	//									break;
	//								}
	//							}
	//						}
	//					}

	//					if (shouldAdd)
	//					{
	//						AddFace(face, vox.type, relPos, relIndex, md, mlod);
	//						relIndex += 4;
	//					}
	//				}
	//			}
	//		}
	//	}

	//}
}