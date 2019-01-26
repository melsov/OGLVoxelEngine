#include "ChunkBuilder.h"
#include "Mesher.h"

void ChunkBuilder::Generate(VoxelAtFunc voxelAt, ChunkSet& cset)
{
	rubiks.set(IDirections::zeroi, cset.GetOrCreate(centerChunkPos));
	try
	{
		rubiks.center()->generateVoxels(voxelAt);
	}
	catch (std::exception e)
	{
		printf("exceptn at: "); DebugVec3(centerChunkPos);
		throw e;
	}

	auto centerPos = rubiks.center()->chunkPosition();
	Chunk *subject;
	ivec3 dir;
	//for (int i = IDirections::FD_DOWN; i < IDirections::FD_NUM_FACE_DIRECTIONS; ++i)
	for(int i = 0; i < 27; ++i)
	{

		dir = IDirections::DirForFaceDir(i);
		subject = cset.GetOrCreate(centerPos + dir);

		if (subject == nullptr)
		{
			printf("ChunkpBuilder subj nullptr: "); DebugVec3(dir + centerPos);
			printf("for center pos: "); DebugVec3(centerPos);
		}

		subject->generateVoxels(voxelAt);
		rubiks.set(dir, subject);
	}
}

void ChunkBuilder::Mesh()
{
	if (!rubiks.center()->getIsMeshDirty())
	{
		return;
	}

	if (!rubiks.hasSixFaceChunksAndCenter())
	{
		throw 7; // no reason for 7. (TODO: learn to use exceptions meaningfully)
	}

	ChunkMesh::MeshData& md = rubiks.center()->meshData;
	ChunkMesh::CreateMeshM(rubiks,  md);

	rubiks.center()->setIsMeshDirty(false);

	md.debug();
}

bool ChunkBuilder::ReadMesh(IOChunk & iochunk)
{

	if (iochunk.ReadConstructDrawableChunk(rubiks.center()->meshData, rubiks.center()->chunkPosition()))
	{
		//TEST
		/*ChunkMesh::MeshData cmd;
		ChunkMesh::CreateMeshM(rubiks, cmd);

		if (ChunkMesh::AssertEqual(rubiks.center()->meshData, cmd))
		{
			std::cout << "&&&&&&&&&&&&&&&& rmd cmd are eq" << std::endl;
		}*/
		// end

		rubiks.center()->setIsMeshDirty(false);
		return true;
	}
	return false;
}

//void ChunkBuilder::Mesh(ChunkMesh::LODMeshDataSet & set)
//{
//	if (!rubiks.center()->getIsDirty())
//	{
//		return;
//	}
//
//	if (!rubiks.hasSixFaceChunksAndCenter())
//	{
//		throw 7;
//	}
//
//	rubiks.center()->setDirty(false);
//	for (int i = 0; i < NUM_LODS; ++i)
//	{
//		ChunkMesh::CreateMeshAtLOD(rubiks, set.mds[i], i);
//	}
//	set.mds[0].debug();
//}

