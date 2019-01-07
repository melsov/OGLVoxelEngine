#include "Mesher.h"
#include "MortonChunk.h"

namespace ChunkMesh
{

	void AddFaceM(
		int FaceDir,
		unsigned char voxType,
		vec3 relPos,
		unsigned short relIndex,
		MeshData& meshData,
		float scale)
	{
		VFace vface;
		AddVFace(
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

	void AddVFace(
		int FaceDir,
		unsigned char voxType,
		vec3 relPos,
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
				vvert.pos[j] = verts[i * 3 + j] / 2.0f * scale + relPos[j];
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


	void CreateMeshM(Cube6::RubiksChunk& rubiks, MeshData& md)
	{
		ivec3 cursor;
		Voxel vox;
		ivec3 relPos;
		int relIndex = 0;

		Chunk* chunk = rubiks.center();


		//OR: compute voxels per view direction on the fly
		//vector<VFace> lodFaceSets[NUM_LODS];
		// stacks per lod of struct VFace-voxPos things
		// iter over the voxels, faces
		//   for lod high to low:
		//      
		//ivec3 lodCursors[NUM_LODS];

		
		//Go through voxels in morton order
		for (int i = 0; i < VOXELS_PER_CHUNK; ++i)
		{
			// go through in morton order 
			// for no reason at the moment ;)
			relPos = MortonChunk::ChunkCoords[i];

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
					AddFaceM(face, vox.type, relPos, relIndex, md);
					relIndex += 4;
				}
			}
		}
		
		/*
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
							AddFaceM(face, vox.type, relPos, relIndex, md);
							relIndex += 4;
						}
					}
				}
			}
		}
		*/
	}

}
