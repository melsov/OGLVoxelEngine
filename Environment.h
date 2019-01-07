#pragma once
#include <map>
#include <glm/glm.hpp>
#include "Chunk.h"

using namespace glm;

class Environment
{
public:
	Environment();
	~Environment();

private:
	std::map<ivec3, Chunk> chunks;
};

