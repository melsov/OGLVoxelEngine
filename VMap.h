#pragma once
#ifndef _VMAP_H_
#define _VMAP_H_

#include <map>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "MathHelper.h"
#include <vector>
#include <memory>

template<class T>
class VMap
{
private:
	std::map<veci3, T> data;

public:
	bool Contains(veci3 chunkPos) const
	{
		return data.find(chunkPos) != data.end(); // count(chunkPos) == 1;
	}

	bool GetSafe(veci3 v, T& chunk)
	{
		if (Contains(v))
		{
			chunk = data.find(v)->second; 
			return true;
		}
		return false;
	}

	bool GetSafe(int x, int y, int z, T& chunk)
	{
		return GetSafe(veci3(x, y, z), chunk);
	}

	void Set(veci3 v, T item)
	{
		data[v] = item;
	}


};


#endif // !_VMAP_H_