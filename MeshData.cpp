#include "MeshData.h"
#include <iostream>

namespace ChunkMesh
{

	bool operator==(const VVertex& v, const VVertex& vv)
	{
		return 
			v.pos[0] == vv.pos[0] &&
			v.pos[1] == vv.pos[1] &&
			v.pos[2] == vv.pos[2] &&

			v.uvs[0] == vv.uvs[0] &&
			v.uvs[1] == vv.uvs[1] &&

			v.color[0] == vv.color[0] &&
			v.color[1] == vv.color[1] &&
			v.color[2] == vv.color[2] &&
			v.color[3] == vv.color[3];
	}

	std::ostream& operator<<(std::ostream &stream, const VVertex v)
	{
		return stream << "pos x: " << v.pos[0] << " y: " << v.pos[1]
			<< " z: " << v.pos[2] << "." << " uv: x: " << v.uvs[0] << " y: " <<
			v.uvs[1] << std::endl <<
			"color: " << v.color[0] << ", " << v.color[1] << ", " << v.color[2] << ", " << v.color[3] << std::endl;
	}


	bool AssertEqual(MeshData& a, MeshData& b)
	{
		using namespace std;

		cout << "************ASSERT EQ****************" << endl;
		if (a.verts.size() != b.verts.size())
		{
			cout << "ver sz neq: a: " << a.verts.size() << " b: " << b.verts.size() << endl;
		}
		for (int i = 0; i < a.verts.size(); ++i)
		{
			if (!(a.verts[i] == b.verts[i]))
			{
				cout << "verts neq at: " << i << endl;
				return false;
			}
		}

		if (a.tris.size() != b.tris.size())
		{
			cout << "tri size neq a: " << a.tris.size() << " b: " << b.tris.size()  << endl;
		}
		else
		for (int i = 0; i < a.tris.size(); ++i)
		{
			cout << "a: " << a.tris[i] << ", b: " << b.tris[i] << endl;

			if (a.tris[i] != b.tris[i])
			{
				cout << "tris neq at: " << i << endl;
				return false;
			}
		}

		cout << "TLOD OFF" << endl;
		for (int i = 0; i < NUM_LODS; ++i)
		{
			cout << "a: " << a.lodTriOffsets[i] << ", b: " << b.lodTriOffsets[i] << endl;
			if (a.lodTriOffsets[i] != b.lodTriOffsets[i])
			{
				cout << "tri off neq at: " << i << endl;
				return false;
			}
		}
		return true;
	}

}