#ifndef _CUBE_6_H_
#define _CUBE_6_H_

#include <glm/glm.hpp>
#include "MathHelper.h"

namespace Cube6
{
	template<class T>
	class Cube6
	{
	public:

		Cube6(T _center) : 
			center(_center),
			storage()
		{
			int numberThatSignifiesBadThings = -8;
			throw numberThatSignifiesBadThings;
		}

		~Cube6() 
		{
		}
		
		//Do we mean T or T& 
		// decide before using this
		T& operator[](int i)
		{
			return storage[i];
		}

		T& operator[](ivec3 v)
		{
			return storage[IndexFor(v)];
		}

		T& get(ivec3 v) { return this[v]; }

		T& get(int x, int y, int z)
		{
			return storage[IndexFor(x, y, z)];
		}

		void set(ivec3 v, const T& val) { this[v] = val; }

		void set(int x, int y, int z, const T& val)
		{
			storage[IndexFor(x, y, z)] = val;
		}

		T& down() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::DOWN]]; }
		T& up() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::UP]]; }
		T& left() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::LEFT]]; }
		T& right() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::RIGHT]]; }
		T& back() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::BACK]]; }
		T& forward() { return this[IDirections.xyz[IDirections::FaceIndexXYZ::FORWARD]]; }
		T& center()
		{
			return center;
		}


	private:
		T storage[6];
		T center;

		int IndexFor(ivec3 v)
		{
			return IndexFor(v.x, v.y, v.z);
		}

		int IndexFor(int x, int y, int z)
		{
			return IDirections::FaceDirectionFor(x, y, z);
		}

	};

}


#endif // !_CUBE_6_H_

