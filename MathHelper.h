#pragma once

#ifndef _MATH_HELPER_
#define _MATH_HELPER_



#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <Windows.h>
#include <stdarg.h>

using namespace glm;


static int inline NegToOnePosToTwoZero(int i)
{
	return i < 0 ? 1 : (i > 0 ? 2 : 0);
}

static int inline ZeroToZeroElseOne(int i)
{
	return i == 0 ? 0 : 1;
}

static int inline ZeroToOneElseZero(int i)
{
	return (i == 0 ? 1 : 0);
}

struct veci3
{
	int x, y, z;

	veci3() {}

	veci3(int xx, int yy, int zz) :
		x(xx), y(yy), z(zz)
	{
	}

	veci3(int i) :
		x(i), y(i), z(i)
	{
	}

	veci3(const glm::ivec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	std::string ToFileNameString()
	{
		std::ostringstream s;
		s << x << "." << y << "." << z;
		return s.str();
	}

	//bool operator <(const veci3& a) const
	//{
	//	return x < a.x && y < a.y && z < a.z;
	//}

#pragma region flat-index

	int toFlatIndexXYZ(const veci3& size)
	{
		return (x * size.y + y) * size.z + z;
	}

#pragma endregion

#pragma region math

	bool allLessThan(const veci3& a) const
	{
		return x < a.x && y < a.y && z < a.z;
	}

	bool allLessOrEqual(const veci3& a) const
	{
		return x <= a.x && y <= a.y && z <= a.z;
	}

	bool allPositive() const
	{
		return x > 0 && y > 0 && z > 0;
	}

	veci3 ZeroMask() const
	{
		return veci3(
			ZeroToOneElseZero(x),
			ZeroToOneElseZero(y),
			ZeroToOneElseZero(z)
		);
	}

	bool allPositiveOrZero() const
	{
		return x >= 0 && y >= 0 && z >= 0;
	}

	int& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: default: return z;
		}
	}

	veci3 operator +(const veci3& a) const
	{
		return veci3(x + a.x, y + a.y, z + a.z);
	}

	void add(const veci3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
	}

	veci3 operator -(const veci3& a) const
	{
		return veci3(x - a.x, y - a.y, z - a.z);
	}

	veci3 operator >>(const int i) const
	{
		return veci3(x >> i, y >> i, z >> i);
	}

	veci3 operator <<(const int i) const
	{
		return veci3(x << i, y << i, z << i);
	}

	void subtract(const veci3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
	}

	// gosh component-wise vector multiplication
	veci3 operator *(const veci3& a) const
	{
		return veci3(x * a.x, y * a.y, z * a.z);
	}

	veci3 operator *(float f) const
	{
		return veci3(x * f, y * f, z * f);
	}

	veci3 operator *(int f) const
	{
		return veci3(x * f, y * f, z * f);
	}

	veci3 operator /(const veci3& a) const
	{
		return veci3(x / a.x, y / a.y, z / a.z);
	}

	veci3 operator /(float f) const
	{
		return veci3(x / f, y / f, z / f);
	}

	veci3 operator %(const veci3 a) const
	{
		return veci3(x % a.x, y % a.y, z % a.z);
	}

	bool operator ==(const veci3& a) const
	{
		return (x == a.x && y == a.y && z == a.z);
	}


	int distanceSq(const veci3& a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}

	int magnitudeSq() const
	{
		return x * x + y * y + z * z;
	}

	int area() const
	{
		return x * y * z;
	}

	veci3 abs() const
	{
		return veci3(std::abs(x), std::abs(y), std::abs(z));
	}

	int shellSize() const
	{
		return area() - (*this - veci3(2)).area();
	}

	veci3 snapToLOD(int lod) const
	{
		return veci3((x >> lod) << lod, (y >> lod) << lod, (z >> lod) << lod);
	}

	bool isOnShell(const veci3& v) const
	{
		return
			v.allPositiveOrZero() &&
			v.allLessThan(*this) &&
			(v.x == 0 ||
				v.y == 0 ||
				v.z == 0 ||
				v.x == x - 1 ||
				v.y == y - 1 ||
				v.z == z - 1);
	}



#pragma endregion

	operator glm::ivec3() const
	{
		return glm::ivec3(x, y, z);
	}

	glm::ivec3 toGLMIVec3() const
	{
		return glm::ivec3(x, y, z);
	}



	// hash the easy way!
	// assume a global limit chunk location XZ.
	// the world is large but finite (sort of).
	// Actually, if we are 2^10 chunks out in a direction
	// we don't see any scenarios where the map will also
	// be holding the chunk +/- 2^10 away...(we hope, heh)

	// or is it better to use std::hash<int>()(x << 1) ^ etc. (or boost hash) as per:
	// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

	int hash() const
	{
		return (x << 20) |
			((z & 1023) << 10) |
			(y & 1023);
	}


}; //END VECI3 def


template<typename T>
static double inline AreaXYZ(const T& v)
{
	return v.x*v.y*v.z;
}

static ivec3 inline Log2Mult(ivec3 v, int log2) {
	return ivec3(v.x << log2, v.y << log2, v.z << log2);
}

static ivec3 inline Log2Div(ivec3 v, int log2) {
	return ivec3(v.x >> log2, v.y >> log2, v.z >> log2);
}

static int inline NegToPosMod(int i, int size) {
	return i >= 0 ? i % size : size + (i + 1) % size - 1;
}


static veci3 inline NegToPosModVeci3(int x, int y, int z, const ivec3& size)
{
	veci3 r;
	r.x = NegToPosMod(x, size.x);
	r.y = NegToPosMod(y, size.y);
	r.z = NegToPosMod(z, size.z);
	return r;
}

static veci3 inline NegToPosMod(int x, int y, int z, int size)
{
	veci3 r;
	r.x = NegToPosMod(x, size);
	r.y = NegToPosMod(y, size);
	r.z = NegToPosMod(z, size);
	return r;
}

static ivec3 inline NegToPosMod(const ivec3& v, const ivec3& size)
{
	ivec3 r;
	r.x = NegToPosMod(v.x, size.x);
	r.y = NegToPosMod(v.y, size.y);
	r.z = NegToPosMod(v.z, size.z);
	return r;
}

static int inline NegToZeroPosToOne(int i)
{
	return i < 0 ? 0 : 1;
}

static ivec3 inline NegToZeroPosToOne(const ivec3& v)
{
	ivec3 r;
	r.x = NegToZeroPosToOne(v.x);
	r.y = NegToZeroPosToOne(v.y);
	r.z = NegToZeroPosToOne(v.z);
	return r;
}


static void inline DebugVec3(const ivec3& v)
{
	printf("v3: %d, %d, %d\n", v.x, v.y, v.z);
}

static void Output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(szBuff);
}

static void inline OutputVec3(const ivec3& v)
{
	Output("v3: %d, %d, %d\n", v.x, v.y, v.z);
}



template<typename T>
int inline FlatXYZIndex(T pos, T size) {
	return (pos.x * size.y + pos.y) * size.z + pos.z;
}

namespace IDirections
{
	static const ivec3 xyz[27] = {
		ivec3(-1, -1, -1), ivec3(0, -1, -1), ivec3(1, -1, -1),
		ivec3(-1,  0, -1), ivec3(0,  0, -1), ivec3(1,  0, -1),
		ivec3(-1,  1, -1), ivec3(0,  1, -1), ivec3(1,  1, -1),

		ivec3(-1, -1,  0), ivec3(0, -1,  0), ivec3(1, -1,  0),
		ivec3(-1,  0,  0), ivec3(0,  0,  0), ivec3(1,  0,  0),
		ivec3(-1,  1,  0), ivec3(0,  1,  0), ivec3(1,  1,  0),

		ivec3(-1, -1,  1), ivec3(0, -1,  1), ivec3(1, -1,  1),
		ivec3(-1,  0,  1), ivec3(0,  0,  1), ivec3(1,  0,  1),
		ivec3(-1,  1,  1), ivec3(0,  1,  1), ivec3(1,  1,  1),

	};

	static const vec3 EightCorners[8] = {
		vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0, 1), vec3(0, 0, 1),
		vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 1, 1), vec3(0, 1, 1),
	};

	namespace FaceIndexXYZ
	{
		enum XYZFaceIndex
		{
			DOWN = 10, UP = 16, LEFT = 12, RIGHT = 14, BACK = 4, FORWARD = 22
		};
	}

	static const ivec3 zeroi(0, 0, 0);
	static const ivec3 onei(1, 1, 1);

	static const ivec3 downi(0, -1, 0);
	static const ivec3 upi(0, 1, 0);
	static const ivec3 lefti(-1, 0, 0);
	static const ivec3 righti(1, 0, 0);
	static const ivec3 backi(0, 0, -1);
	static const ivec3 forwardi(0, 0, 1);

	static const veci3 zerovi(0, 0, 0);
	static const veci3 onevi(1, 1, 1);

	static const veci3 downvi(0, -1, 0);
	static const veci3 upvi(0, 1, 0);
	static const veci3 leftvi(-1, 0, 0);
	static const veci3 rightvi(1, 0, 0);
	static const veci3 backvi(0, 0, -1);
	static const veci3 forwardvi(0, 0, 1);

	static const ivec3 ThreePosDirs[3] = {
		upi, righti, forwardi
	};

	enum FaceDirection
	{
		FD_DOWN, FD_UP, FD_LEFT, FD_RIGHT, FD_BACK, FD_FORWARD, FD_NUM_FACE_DIRECTIONS
	};

	static const char* FaceDirNames[6] = {
		"down", "up", "left", "right", "back", "forward"
	};

	static int FaceDirectionFor(int x, int y, int z)
	{
		if (y < 0) return FD_DOWN;
		if (y > 0) return FD_UP;
		if (x < 0) return FD_LEFT;
		if (x > 0) return FD_RIGHT;
		if (z < 0) return FD_BACK;
		return FD_FORWARD;
	}

	static int FaceDirectionFor(ivec3 v)
	{
		return FaceDirectionFor(v.x, v.y, v.z);
	}

	static ivec3 DirForFaceDir(int fd)
	{
		switch (fd)
		{
		case FD_DOWN: return downi;
		case FD_UP: return upi;
		case FD_LEFT: return lefti;
		case FD_RIGHT: return righti;
		case FD_BACK: return backi;
		case FD_FORWARD: return forwardi;
		default: return zeroi;
		}
	}

	static veci3 VIDirForFaceDir(int fd)
	{
		switch (fd)
		{
		case FD_DOWN: return downvi;
		case FD_UP: return upvi;
		case FD_LEFT: return leftvi;
		case FD_RIGHT: return rightvi;
		case FD_BACK: return backvi;
		case FD_FORWARD: return forwardvi;
		default: return zerovi;
		}
	}

	static bool IsNegativeDirection(int fd)
	{
		switch (fd)
		{
		case FD_DOWN:
		case FD_LEFT:
		case FD_BACK:
			return true;
		}
		return false;
	}

	static ivec3 PerpDirForFaceDir(int fd, bool which)
	{
		switch (fd)
		{
		case FD_DOWN:
		case FD_UP:
			return which ? righti : forwardi;
		case FD_LEFT:
		case FD_RIGHT:
			return which ? forwardi : upi;
		case FD_BACK:
		case FD_FORWARD:
		default:
			return which ? upi : righti;
		}
	}

	

}



inline 
ivec3 SnapToLOD(int lod, ivec3 v)
{
	return ivec3((v.x >> lod) << lod, (v.y >> lod) << lod, (v.z >> lod) << lod);
}

inline
veci3 SnapToLOD(int lod, veci3 v)
{
	return veci3((v.x >> lod) << lod, (v.y >> lod) << lod, (v.z >> lod) << lod);
}

//ToString
inline
std::ostream& operator<<(std::ostream &stream, const veci3 v)
{
	return stream << "veci3 x: " << v.x << " y: " << v.y << " z: " << v.z << ".";
}

inline
std::ostream& operator<<(std::ostream &stream, const vec3 v)
{
	return stream << "veci3 x: " << v.x << " y: " << v.y << " z: " << v.z << ".";
}

namespace NFlatIndex
{
	static veci3 FromFlatIndexXYZ(int idx, veci3 size)
	{
		return veci3(
			idx / (size.y * size.z),
			(idx / size.z) % size.y,
			idx % size.z
		);
	}
}

namespace std
{
	// required for unordered maps
	template <> 
	struct hash<veci3>
	{
		size_t operator()(const veci3& k) const
		{
			return k.hash();
		}
	};

	// for ordered maps
	template<>
	struct less<veci3>
	{
		bool operator() (const veci3& a, const veci3& b) const
		{
			return a.hash() < b.hash();
		}
	};
}


#endif // !_MATH_HELPER_