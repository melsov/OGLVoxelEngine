#include "IOChunk.h"
#include "WorldConfig.h"

//TODO: copy tom dall's cross platform get file
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <sstream>
#include "platform.h"

#include <filesystem>
#include <math.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace fs = std::filesystem;
//#include <boost/filesystem.hpp>

IOChunk::IOChunk() : IOChunk(DEFAULT_WORLD_NAME)
{
}

IOChunk::IOChunk(std::string _worldName) 
	//: bpath(fs::current_path() / "ChunkData" / _worldName)
{
	std::ostringstream s;
	s << ResourcePath("") << "../ChunkData/" << _worldName;
	bpath = s.str();

	//boost::filesystem::create_directory(bpath);
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

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive& ar, ChunkMesh::VVertex& v, const unsigned int version)
		{
			ar & v.pos[0];
			ar & v.pos[1];
			ar & v.pos[2];
			ar & v.uvs[0];
			ar & v.uvs[1];
			ar & v.color[0];
			ar & v.color[1];
			ar & v.color[2];
			ar & v.color[3];
		}
	}
}

typedef std::vector<ChunkMesh::VVertex> VertexList;

void TestBoostVVSer()
{

	// create and open a character archive for output
	std::ofstream ofs("filename", std::fstream::binary | std::fstream::out);

	// create class instance
	const ChunkMesh::VVertex g = ChunkMesh::FakeVVertex(); // (35, 59, 24.567f);

	VertexList vin;
	vin.push_back(g);

	std::cout << "orig: " << g;

	// save data to archive
	{
		//boost::archive::text_oarchive oa(ofs);
		boost::archive::binary_oarchive oa(ofs);
		// write class instance to archive
		oa << vin;
		// archive and stream closed when destructors are called
	}


	VertexList vout;

	// ... some time later restore the class instance to its orginal state
	{
		//ChunkMesh::VVertex newg;
		// create and open an archive for input
		std::ifstream ifs("filename", std::fstream::binary | std::fstream::in);
		boost::archive::binary_iarchive ia(ifs);
		// read class state from archive
		ia >> vout;
		// archive and stream closed when destructors are called

		std::cout << " vout size: " << vout.size();

		std::cout << "read: " << vout[0];
	}

}

namespace SerChunk
{
#define FTOINT 255.0F
//#define ONE_OVER_255 0.01f
#define ONE_OVER_255 0.00392156862f

	struct SerPos
	{
		char whole, decimal;

		SerPos(){}

		SerPos(GLfloat f) 
		{
			/*int exp;
			float d = frexp(f, &exp);
			whole = ((int)(d * FTOINT));
			ex = exp;*/
			float i;
			float d = modf(f, &i);
			whole = i;
			decimal = d * FTOINT;
		}

		operator GLfloat() const
		{
			return whole + decimal * ONE_OVER_255;
			//return ldexp((float)whole * ONE_OVER_255, (int)ex); // whole + ex * ONE_OVER_255;
		}
	};

	struct Ser01
	{
		char decimal;

		Ser01(){}

		Ser01(GLfloat f) 
		{ 
			float exp;
			decimal = (int)(modf(f, &exp) * FTOINT);
			
		}

		operator GLfloat() const
		{
			return decimal * ONE_OVER_255;
		}
	};

	struct SerVVertex
	{
		SerPos pos[3];
		Ser01 uvs[2];
		Ser01 color[4];

		SerVVertex(ChunkMesh::VVertex vv)
		{
			for (int i = 0; i < 3; ++i)
			{
				pos[i] = SerPos(vv.pos[i]);
			}
			for (int i = 0; i < 2; ++i)
			{
				uvs[i] = Ser01(vv.uvs[i]);
			}
			for (int i = 0; i < 4; ++i)
			{
				color[i] = Ser01(vv.color[i]);
			}
		}

		operator ChunkMesh::VVertex() const
		{
			ChunkMesh::VVertex vv;
			for (int i = 0; i < 3; ++i)
			{
				vv.pos[i] = pos[i];
			}
			for (int i = 0; i < 2; ++i)
			{
				vv.uvs[i] = uvs[i];
			}
			for (int i = 0; i < 4; ++i)
			{
				vv.color[i] = color[i];
			}
			return vv;
		}

	};
}

void testEXPs(GLfloat f)
{
	using namespace std;

	int exp;
	float sig = frexp(f, &exp);

	char csig = (int)(sig * FTOINT);
	float fsig = csig * ONE_OVER_255;
	char ex = exp;

	//std::cout << "ex: " << (int)ex << " exp: " << exp;
	float fa = ldexp(fsig, (int)ex);


	//std::cout << "f: " << f << " fa: " << fa << std::endl;

	float i;
	float frac = modf(f, &i);

	char ci = (int)i;
	char cf = (int)(frac * FTOINT);
	
	float fcf = (int)cf * ONE_OVER_255;
	float fout = ci + fcf;

	cout << "f : " << f << endl;
	cout << "frac: " << frac << " cf: " << cf << " fcf: " << fcf << endl;
	cout << "fout: " << fout << endl;



}

void TestSerVV()
{
	/*testEXPs(0.0f);
	testEXPs(1.2f);
	testEXPs(-72.5f);
	testEXPs(-999999.7f);*/
	TestBoostVVSer();
	return;

	for (int i = 0; i < 16; ++i)
	{
		testEXPs(i);
		testEXPs(i + .9f);
	}
	

	auto vv = ChunkMesh::FakeVVertex();

	SerChunk::SerVVertex sv = vv;

	GLfloat dif;
	ChunkMesh::VVertex s = sv;
	for (int i = 0; i < 3; ++i)
	{
		 dif = vv.pos[i] - s.pos[i];
		 std::cout << "vv: " << vv.pos[i] << " s: " << s.pos[i] << " dif " << i << ": " << dif << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 2; ++i)
	{
		dif = vv.uvs[i] - s.uvs[i];
		std::cout << "vv: " << vv.uvs[i] << " s: " << s.uvs[i] << " dif " << i << ": " << dif << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		dif = vv.color[i] - s.color[i];
		std::cout << "vv: " << vv.color[i] << " s: " << s.color[i] << " dif " << i << ": " << dif << std::endl;
	}
	std::cout << std::endl;
}

void DoVertThing()
{
	ChunkMesh::VVertex hh = ChunkMesh::FakeVVertex();

	std::cout  << "HH pos 0 :"<< hh.pos[0] <<
		//" " << h.length << " " << h.count << 
		std::endl;

	std::fstream fh;
	fh.open("test.txt", std::fstream::out | std::fstream::binary);
	fh.write((char*)&hh, sizeof(ChunkMesh::VVertex));
	fh.close();

	fh.open("test.txt", std::fstream::in | std::fstream::binary);

	ChunkMesh::VVertex h;
	fh.read((char*)&h.pos, sizeof(h.pos) / sizeof(GLfloat));
	//fh.read((char*)&h.length, sizeof(h.length));
	//fh.read((char*)&h.count, sizeof(h.count));

	fh.close();

	std::cout << " h pos 0: " << h.pos[0] << 
		//" " << h.length << " " << h.count << 
		std::endl;
}

void IOChunk::Test()
{
	//DoSOThing(); return;
	//DoVertThing(); return;
	TestSerVV(); return;

	int howMany = 5;
	//std::vector<int> data(howMany);
	int data[5];
	for (int i = 0; i < howMany; ++i)
	{
		data[i] = i;
	}

	std::ostringstream s;
	s << bpath << "/test.txt";
	std::basic_ofstream<int> ofp(s.str(), std::fstream::out | std::fstream::binary);
	ofp.write((&data[0]), sizeof(data) * sizeof(data[0]));
	ofp.close();

	std::ifstream ifp(s.str(), std::fstream::in | std::fstream::binary);
	if (!ifp.is_open())
	{
		std::cout << "file not open" << std::endl;
		return;
	}

	std::string pa = ResourcePath("test.txt");
	std::cout << "got pa: " << pa << std::endl;

	std::basic_ifstream<int> stream(s.str(), std::ios::in | std::ios::binary);
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

std::string IOChunk::PathFor(veci3 cpos, bool drawable)
{
	std::ostringstream s;
	s << bpath << "/";
	s << (drawable ? "drawChunk" : "chunk") << cpos.ToFileNameString() << ".hello";
	return s.str();
}

std::string Append(std::string a, std::string b)
{
	std::ostringstream s;
	s << a << b;
	return s.str();
}

void IOChunk::WriteDrawableChunk(ChunkMesh::MeshData & md, veci3 cpos)
{
	auto cfile = PathFor(cpos, true);
	//TRI OFFSETS
	{
		std::ofstream ofp(Append(cfile, ".off"), std::fstream::out | std::fstream::binary);
		boost::archive::binary_oarchive oa(ofp);
		oa << md.lodTriOffsets;
		ofp.close();

		//auto cfileOff = Append(cfile, ".off");
		//int size = sizeof(md.lodTriOffsets) / sizeof(md.lodTriOffsets[0]);
		//std::cout << "write size array: " << sizeof(md.lodTriOffsets) << " size of [0]: " << sizeof(md.lodTriOffsets[0]) << std::endl;

		//std::basic_ofstream<int> ofp(cfileOff, std::fstream::out | std::fstream::binary);
		//ofp.write(&md.lodTriOffsets[0], size);
		//ofp.close();
	}

	//TRIS
	{
		std::ofstream ofp(Append(cfile, ".tris"), std::fstream::out | std::fstream::binary);
		boost::archive::binary_oarchive oa(ofp);
		oa << md.tris;
		ofp.close();
		
		/*std::basic_ofstream<TRI_T> ofp(Append(cfile, ".tris"), std::fstream::out | std::fstream::binary);
		ofp.write(md.tris.data(), md.tris.size());
		ofp.close();*/
	}

	//VERTS
	{
		std::ofstream ofp(Append(cfile, ".verts"), std::fstream::out | std::fstream::binary);
		boost::archive::binary_oarchive oa(ofp);
		oa << md.verts;
		ofp.close();
	}

}

bool IOChunk::ReadTriOffsets(ChunkMesh::MeshData& md, veci3 cpos)
{
	auto cfile = PathFor(cpos, true);

	std::ifstream ifc(Append(cfile, ".off"), std::fstream::in | std::fstream::binary);
	if (!ifc.is_open()) { return false; }

	boost::archive::binary_iarchive ia(ifc);
	ia >> md.lodTriOffsets;
	ifc.close();
	return true;

	/*auto cfile = PathFor(cpos, true);
	auto cfileoff = Append(cfile, ".off");

	std::basic_ifstream<int> ifc(cfileoff, std::fstream::in || std::fstream::binary);

	if (!ifc.is_open()) { return false; }

	auto eos = std::istreambuf_iterator<int>();
	auto buffer = std::vector<int>(std::istreambuf_iterator<int>(ifc), eos);

	for (int i = 0; i < buffer.size(); ++i)
	{
		std::cout << i << ": " << buffer[i] << ", ";
	}std::cout << std::endl;

	ifc.seekg(0, std::ios::end);
	std::streampos size = ifc.tellg();
	if (size != NUM_LODS)
	{
		std::cout << "wrong stream char/byte size : " << size << std::endl; return false;
	}
	ifc.seekg(0);

	ifc.read(md.lodTriOffsets, NUM_LODS * sizeof(int));

	ifc.close();
	return true;*/
}

bool IOChunk::ReadTris(ChunkMesh::MeshData& md, veci3 cpos)
{

	auto cfile = PathFor(cpos, true);

	std::ifstream ifc(Append(cfile, ".tris"), std::fstream::in | std::fstream::binary);
	if (!ifc.is_open()) { return false; }

	boost::archive::binary_iarchive ia(ifc);
	ia >> md.tris;
	ifc.close();
	return true;

	//auto cfileoff = Append(cfile, ".tris");

	//std::basic_ifstream<TRI_T> ifc(cfileoff, std::fstream::in || std::fstream::binary);

	//if (!ifc.is_open()) { return false; }

	//auto eos = std::istreambuf_iterator<TRI_T>();
	////auto buffer = std::vector<TRI_T>(std::istreambuf_iterator<TRI_T>(ifc), eos);
	//md.tris = std::vector<TRI_T>(std::istreambuf_iterator<TRI_T>(ifc), eos);


	//std::cout << "*** tris: ";
	//for (int i = 0; i < md.tris.size(); ++i)
	//{
	//	std::cout << i << ": " << md.tris[i] << ", ";
	//}std::cout << std::endl;


	//ifc.close();
	//return true;
}

bool IOChunk::ReadVerts(ChunkMesh::MeshData& md, veci3 cpos)
{
	auto cfile = PathFor(cpos, true);
	std::ifstream ifc(Append(cfile, ".verts"), std::fstream::in | std::fstream::binary);
	if (!ifc.is_open()) { return false; }

	boost::archive::binary_iarchive ia(ifc);
	ia >> md.verts;
	ifc.close();
	return true;
}

bool IOChunk::ReadConstructDrawableChunk(ChunkMesh::MeshData& md, veci3 cpos)
{
	if (!ReadTriOffsets(md, cpos)) { return false; }
	if (!ReadTris(md, cpos)) { return false; }
	if (!ReadVerts(md, cpos)) { return false; }
	return true;
}

void IOChunk::WriteChunk(Chunk& chunk)
{
	auto cfile = PathFor(chunk.chunkPosition(), false);
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
	chunk.SetBuildStage(BCSHAS_GENERATED);
	chunk.updateIsEmpty();
	chunk.Debug();

	return true;
}

void IOChunk::DebugDestroyWorldData()
{
	// fs::remove_all(bpath);
}
