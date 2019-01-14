#include "Mesher.h"
#include "MortonChunk.h"
#include "LODFaceCoords.h"
#include <assert.h>

namespace ChunkMesh
{



	void AddVFace_(
		int FaceDir,
		unsigned char voxType,
		veci3 relPos,
		unsigned short relIndex,
		//MeshData & meshData, 
		VFace& vface,
		float scale)
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

		//TODO:
		// foreach vert look up the two cross voxels
		// that might make a corner shadow

		auto offset = TileOffsetForVoxType(voxType, FaceDir);
		int i, j;
		for (i = 0; i < 4; ++i)
		{
			VVertex vvert;
			for (j = 0; j < 3; ++j)
			{
				//vvert.pos[j] = verts[i * 3 + j] / 2.0f * scale + relPos[j];
				vvert.pos[j] = (verts[i * 3 + j] < 0 ? 0.0f : .9f) * scale + relPos[j];
			}
			for (j = 0; j < 2; ++j)
			{
				vvert.uvs[j] = (j == 1 ? offset.y : offset.x) + uvFace[i * 2 + j];
			}
			GLfloat color[] = { 1, .7, .6, 1 };
			for (j = 0; j < 4; ++j)
			{
				vvert.color[j] = color[j];
			}
			vface.verts[i] = vvert;
			//meshData.verts.push_back(vvert);
		}

		for (int i = 0; i < 6; ++i)
		{
			vface.tris[i] = triFace[i] + relIndex;
			//meshData.tris.push_back(triFace[i] + relIndex);
		}
	}

	void AddFaceM_(
		int FaceDir,
		unsigned char voxType,
		veci3 relPos,
		unsigned short relIndex,
		MeshData& meshData,
		float scale = 1.0f)
	{
		VFace vface;
		AddVFace_(
			FaceDir,
			voxType,
			relPos,
			relIndex,
			vface,
			scale);

		for (int i = 0; i < 4; ++i)
		{
			meshData.verts.push_back(vface.verts[i]);
		}

		for (int i = 0; i < 6; ++i)
		{
			meshData.tris.push_back(vface.tris[i]);
		}

	}

	//
	// Builds a chunk mesh
	// Orders faces by grouping according to LOD.
	// If a voxel is the first found within a given LOD cube (e.g. sizes 4, 2, 1),
	// its faces go into that LOD group. Iterates over voxels in Morton order
	// to guarantee visiting LOD cubes sequentially
	//
	void CreateMeshM(Cube6::RubiksChunk& rubiks, MeshData& md)
	{
		veci3 cursor;
		Voxel vox;
		veci3 relPos;
		int relIndex = 0;

		Chunk* chunk = rubiks.center();
		
		// CONSIDER: this LOD strategy will add (hopefully not too many) faces
		// since chunkier LOD levels have more chances to be visible
		// and yet these extra faces will carry over to lower lods
		// for now: 'oh well'

		MeshData mds[NUM_LODS];
		int mortonLevel; 
		veci3 mortonCursor = MortonChunk::ChunkCoords[0]; // (0,0,0)
		veci3 lastFound[NUM_LODS];
		for(int i = 0 ; i < NUM_LODS; ++i) lastFound[i] = IDirections::backi * 999999;
		
		for (int i = 0; i < VOXELS_PER_CHUNK; ++i)
		{
			// go through in morton order 
			relPos = MortonChunk::ChunkCoords[i];
			
			mortonLevel = NUM_LODS - 1;

			// already found a voxel for this lod? decrement mortonLevel
			while (MortonChunk::IsSameMortonCube(relPos, lastFound[mortonLevel], mortonLevel))
			{
				if (mortonLevel == 0) break;
				mortonLevel--;
			}

			mortonCursor = SnapToLOD(mortonLevel, relPos);

			chunk->voxelAtSafe(relPos.x, relPos.y, relPos.z, vox);

			if (vox.type <= VEMPTY) { continue; }

			int face;
			for (face = FACE_DOWN; face <= FACE_FORWARD; ++face)
			{
				//visible at this lod?

				bool shouldAdd = true;

				// Don't miss positive dir faces that wont show at higher morton
				// but will show at lower morton (i.e. annoying case where 4 above a face is 
				// a solid ceiling but air in between.) 
				// Therefore, for positive faces, try each lod from to mortonLevel to zero
				for (int lvl = mortonLevel; lvl >= (IDirections::IsNegativeDirection(face) ? mortonLevel : 0); --lvl)
				{

					//auto lodfaceIncrs = IDirections::LodFaceLookup.faces[mortonLevel][face];
					auto lodfaceIncrs = IDirections::LodFaceLookup.faces[lvl][face];

					// for positive direction, look for voxels at the 'outer' edge of the LOD cube
					// for example: (face == RIGHT, mortonLevel = 2) move to pos (3, 0, 0) (i.e. lower right side voxel)
					veci3 nudgePos = ((veci3)IDirections::DirForFaceDir(face)) *
						(IDirections::IsNegativeDirection(face) ? 0 : (1 << lvl) - 1);

					veci3 lvlCursor = SnapToLOD(lvl, relPos);

					for (auto it = lodfaceIncrs.begin(); it != lodfaceIncrs.end(); ++it)
					{
						cursor = (lvlCursor + nudgePos + *it).toGLMIVec3();
						if (rubiks.voxelAtSafe(cursor, vox))
						{
							shouldAdd = vox.type == VEMPTY;
							if (shouldAdd)
							{
								for (int l = mortonLevel; l >= 0; --l) lastFound[l] = mortonCursor;
								break;
							}
						}
					}
					/*
					// OLD visible (no LOD)
					cursor = FromDir(face) + relPos;
					if (rubiks.voxelAtSafe(cursor, vox))
					{
						shouldAdd = vox.type == VEMPTY;
					}
					*/

					if (shouldAdd)
					{
						AddFaceM_(face, vox.type, relPos, relIndex, mds[mortonLevel]);
						relIndex += 4;
						break;
					}
				}
			}

			
		}

		//copy the mesh data into md
		int triOffset = 0;
		for (int i = NUM_LODS - 1; i >= 0; --i)
		{
			for (auto it = mds[i].verts.begin(); it != mds[i].verts.end(); ++it)
			{
				md.verts.push_back(*it);
			}
			for (auto it = mds[i].tris.begin(); it != mds[i].tris.end(); ++it)
			{
				md.tris.push_back(*it + triOffset);
			}
			md.lodTriOffsets[NUM_LODS - 1 - i] = mds[i].tris.size();
			triOffset += md.lodTriOffsets[NUM_LODS - 1 - i];
		}
		
		
	}

}
