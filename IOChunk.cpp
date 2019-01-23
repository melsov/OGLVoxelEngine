#include "IOChunk.h"
#include "WorldConfig.h"

#include <filesystem>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <sstream>


namespace fs = std::filesystem;

IOChunk::IOChunk() : IOChunk(DEFAULT_WORLD_NAME)
{
}

IOChunk::IOChunk(std::string _worldName) :
	bpath(fs::current_path() / "ChunkData" / _worldName)
{
	fs::create_directory(bpath);
}


IOChunk::~IOChunk()
{
}

#pragma pack(push, r1, 1)
struct Header
{
	char id[15];
	int length;
	int count;
};
#pragma pack(pop, r1)

void DoSOThing()
{
	Header hh = { "alalalala", 5, 10 };

	std::fstream fh;
	fh.open("test.txt", std::fstream::out | std::fstream::binary);
	fh.write((char*)&hh, sizeof(Header));
	fh.close();

	fh.open("test.txt", std::fstream::in | std::fstream::binary);

	Header h;
	fh.read((char*)&h.id, sizeof(h.id));
	fh.read((char*)&h.length, sizeof(h.length));
	fh.read((char*)&h.count, sizeof(h.count));

	fh.close();

	std::cout << h.id << " " << h.length << " " << h.count << std::endl;
}

void IOChunk::Test()
{
	//DoSOThing(); return;


	int howMany = 5;
	//std::vector<int> data(howMany);
	int data[5];
	for (int i = 0; i < howMany; ++i)
	{
		data[i] = i;
	}

	std::basic_ofstream<int> ofp(bpath / "test.txt", std::fstream::out | std::fstream::binary);
	ofp.write((&data[0]), sizeof(data) * sizeof(data[0]));
	ofp.close();

	std::ifstream ifp(bpath / "test.txt", std::fstream::in | std::fstream::binary);
	if (!ifp.is_open())
	{
		std::cout << "file not open" << std::endl;
		return;
	}

	std::basic_ifstream<int> stream(bpath / "test.txt", std::ios::in | std::ios::binary);
	auto eos = std::istreambuf_iterator<int>();
	auto buffer = std::vector<int>(std::istreambuf_iterator<int>(stream), eos);

	for (int i = 0; i < howMany; ++i)
	{
		std::cout << buffer[i] << ", ";
	}
	std::cout << "risky? " << std::endl;

	stream.close();

	std::vector<int> indata;
	std::for_each(std::istreambuf_iterator<char>(ifp),
		std::istreambuf_iterator<char>(),
		[&indata](const char c)
	{
		indata.push_back(c);
	});


	//int indata[5]; // = new int[howMany];
	//std::cout << "size of indata " << sizeof(indata) << std::endl;
	//ifp.read((char*)(&indata[0]), howMany * sizeof(int));
	for (int i = 0; i < howMany; ++i)
	{
		int f = indata[i];
		std::cout << f << ", ";
	}
	//delete[] indata;

	ifp.close();

	//std::vector<int> buff(std::istream_iterator<char>(ifp), {});
	//int j = 0;
	//for (auto it = buff.begin(); it != buff.end(); ++it)
	//{
	//	std::cout << j++ << " : "  << *it << std::endl;
	//}

	/*ifp.seekg(0, std::ios::end );
	std::streampos size = ifp.tellg();
	std::cout << "size : " << size << std::endl;
	ifp.seekg(0);

	float* indata = new float[10];
	ifp.read(reinterpret_cast<char *>(indata), sizeof(indata) * sizeof(float));

	for (int i = 0; i < 10; ++i)
	{
		float f = indata[i];
		std::cout << f << ", ";
	}

	delete[] indata;*/
}

std::filesystem::path IOChunk::PathFor(veci3 cpos, bool drawable)
{
	std::ostringstream s;
	s << (drawable ? "drawChunk" : "chunk") << cpos.ToFileNameString() << ".hello";
	return (bpath / s.str()).string();
}

void IOChunk::WriteDrawableChunk(DrawableChunk & dc, veci3 cpos)
{
}

void IOChunk::WriteChunk(Chunk& chunk)
{
	std::cout << "." << std::endl;
	fs::path cfile = PathFor(chunk.chunkPosition(), false);
	Voxel* data = chunk.getData();
	int size = chunk.getSizeBytes();
	if (size == 0) { return; }
	std::cout << "writing " << ((veci3) chunk.chunkPosition()) << std::endl;

	std::cout << cfile << std::endl;
	std::basic_ofstream<unsigned char> ofp(cfile, std::fstream::out | std::fstream::binary);
	ofp.write((unsigned char *) &(data[0]), size);
	ofp.close();
}

bool IOChunk::ReadConstructChunk(Chunk& chunk)
{
	
	auto cfile = PathFor(chunk.chunkPosition(), false);
	std::basic_ifstream<unsigned char> ifc(cfile, std::fstream::in || std::fstream::binary);

	if (!ifc.is_open()) 
	{ 
		std::cout << "no file at: " << cfile << std::endl;
		return false; 
	}

	ifc.seekg(0, std::ios::end);
	std::streampos size = ifc.tellg();
	if (size != VOXELS_PER_CHUNK)
	{
		std::cout << "wrong stream char/byte size : " << size << std::endl;
		return false;
	}
	ifc.seekg(0);

	std::cout << "reading" << std::endl;
	unsigned char *stor = new unsigned char[VOXELS_PER_CHUNK];
	ifc.read(stor, VOXELS_PER_CHUNK * sizeof(Voxel));
	ifc.close();

	chunk.TakeVoxelArray((Voxel*) stor);
	//chunk.setIsMeshDirty(false);
	chunk.SetBuildStage(BCSHAS_GENERATED);
	chunk.updateIsEmpty();
	chunk.Debug();

	return true;
}

void IOChunk::DebugDestroyWorldData()
{
	// fs::remove_all(bpath);
}
