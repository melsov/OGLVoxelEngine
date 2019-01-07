#ifndef _RUBIKS_CONTAINER_H_
#define _RUBIKS_CONTAINER_H_


#include <glm/glm.hpp>
#include "MathHelper.h"

using namespace glm;

namespace Cube6
{

	template<class RT>
	class RubiksContainer
	{
	public:

		RubiksContainer() : storage() 
		{}

		~RubiksContainer() 
		{ }


		RT operator[](int i)
		{
			return storage[i];
		}

		RT operator[](ivec3 v)
		{
			return storage[IndexFor(v)];
		}

		RT get(ivec3 v) { return storage[IndexFor(v)]; }

		RT get(int x, int y, int z)
		{
			return storage[IndexFor(x, y, z)];
		}

		void set(ivec3 v, const RT val) { 
			storage[IndexFor(v)] = val; 
		}

		void set(int x, int y, int z, const RT val)
		{
			storage[IndexFor(x, y, z)] = val;
		}

		RT getWithFaceDir(int fd)
		{
			return get(IDirections::DirForFaceDir(fd));
		}

		void setWithFaceDir(int fd, const RT& val)
		{
			set(IDirections::DirForFaceDir(fd), val);
		}

		RT down() { return get( IDirections::xyz[IDirections::FaceIndexXYZ::DOWN]); }
		RT up() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::UP]); }
		RT left() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::LEFT]); }
		RT right() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::RIGHT]); }
		RT back() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::BACK]); }
		RT forward() { return get(IDirections::xyz[IDirections::FaceIndexXYZ::FORWARD]); }
		RT center() { return get(IDirections::zeroi); }


		static const int SIZE = 27;

		void Debug()
		{
			int nulls = 0;
			for (int i = 0; i < 27; ++i)
			{
				if (storage[i] == nullptr)
				{
					nulls++;
				}
				else
				{
					printf("non null item\n");
					RT c = storage[i];
					//c->Debug();
				}
			}
			printf("null count: %d \n", nulls);
		}

	private:
		RT storage[27];

		int IndexFor(ivec3 v)
		{
			return IndexFor(v.x, v.y, v.z);
		}

		int IndexFor(int x, int y, int z)
		{
			return ++x * 9 + ++y * 3 + ++z;
		}

	};


}
#endif // !_RUBIKS_CONTAINER_H_
