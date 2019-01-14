#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "DrawableChunk.h"

#include "shader.hpp" //<shader.hpp>
#include "texture.hpp" //<texture.hpp>
//#include "controls.hpp"
#include "RubiksContainer.h"
#include "FlatPow2Array3D.h"
#include "Chunk.h"
#include "ChunkMesh.h"
#include <vector>
#include "SOIL.h"
#include "MathHelper.h"
#include "ChunkSet.h"
#include "ChunkBuilder.h"
#include "ChunkBuildPool.h"
//#include <string.h>
#include <memory>
#include "VMap.h"
#include <thread>
#include <chrono>
#include "KeyboardDebouncer.h"
#include "ProximityList.h"
#include "ChunkProximity.h"
#include "CamData.h"
#include "MortonChunk.h"
#include "LODFaceCoords.h"



veci3 fakePlayerPos(HALF_CHUNK_SIZE);

void updateFakePlayerPos(int key)
{
	veci3 dir(0);
	switch (key)
	{
	case GLFW_KEY_B:
		dir = IDirections::righti; break;
	case GLFW_KEY_C:
		dir = IDirections::lefti; break;
	case GLFW_KEY_F:
		dir = IDirections::upi; break;
	case GLFW_KEY_V:
		dir = IDirections::downi; break;
	default:
		break;
	}

	fakePlayerPos = fakePlayerPos + (dir * CHUNK_SIZE);

	if (!(dir == veci3(0)))
	{
		printf("fake plyr pos: "); DebugVec3(fakePlayerPos.toGLMIVec3());
	}

}

veci3 providePlayerPos() { return fakePlayerPos; }



void TestSomething() 
{
	// test code here

	

	// end test code

	//while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); 
}

Voxel UpsideDownQuarterPyramid(int x, int y, int z)
{
	Voxel result;
	x %= CHUNK_SIZE;
	y %= CHUNK_SIZE;
	z %= CHUNK_SIZE;
	x = abs(x);
	y = abs(y);
	z = abs(z);
	if (x + y + z < CHUNK_SIZE + 3)
	{
		result.type = VDIRT;
	}
	else
	{
		result.type = VEMPTY;
	}
	return result;
}

Voxel CenteredCubes(int x, int y, int z)
{
	Voxel result;
	x = NegToPosMod(x, CHUNK_SIZE);
	y = NegToPosMod(y, CHUNK_SIZE);
	z = NegToPosMod(z, CHUNK_SIZE);

	int mid = CHUNK_SIZE / 2;
	x = mid - x;
	y = mid - y;
	z = mid - z;

	int rad = 0;
	if (x > rad || y > rad || z > rad)
	{
		result.type = VEMPTY;
	}
	else
	{
		result.type = VGRASS;
	}
	return result;
}

Voxel CornerCubes(int x, int y, int z)
{
	Voxel result;
	x = NegToPosMod(x, CHUNK_SIZE);
	y = NegToPosMod(y, CHUNK_SIZE);
	z = NegToPosMod(z, CHUNK_SIZE);



	int rad = 8;
	if (x >= rad || y >= rad || z >= rad)
	{
		result.type = VEMPTY;
	}
	else
	{
		result.type = VGRASS;
	}
	return result;
}

Voxel XZBars(int x, int y, int z)
{
	Voxel result;
	x = NegToPosMod(x, CHUNK_SIZE);
	y = NegToPosMod(y, CHUNK_SIZE);
	z = NegToPosMod(z, CHUNK_SIZE);

	int s = 3;

	veci3 pos(x, y, z);
	veci3 barZ(s, s, CHUNK_SIZE);
	veci3 barX(CHUNK_SIZE, s, s);

	if (pos.allLessOrEqual(barZ) || pos.allLessOrEqual(barX))
	{
		result.type = VGRASS;
	}
	else
	{
		result.type = VEMPTY;
	}
	return result;
}

Voxel CornerCubesWithConnectors(int x, int y, int z)
{
	Voxel result;
	x = NegToPosMod(x, CHUNK_SIZE);
	y = NegToPosMod(y, CHUNK_SIZE);
	z = NegToPosMod(z, CHUNK_SIZE);

	int rad = 8;

	if (x < rad || y < rad || z < rad)
	{
		result.type = VGRASS;
	}
	else
	{
		//connectors
		int xd, yd, zd;
		xd = abs(rad - x);
		yd = abs(rad - y);
		zd = abs(rad - z);

		if (xd + yd < 3 || xd + zd < 3 || yd + zd < 3)
		{
			result.type = VGRASS;
		}
		else
		{
			result.type = VEMPTY;
		}

	}
	return result;
}




Voxel TestGetVoxel(int x, int y, int z)
{
	//return XZBars(x, y, z);
	//return CornerCubes(x, y, z);
	//return CornerCubesWithConnectors(x, y, z);
	//return CenteredCubes(x, y, z);
	return UpsideDownQuarterPyramid(x, y, z);
}



int initGLFWAndGLEW()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init glfw ");
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "This is a window", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	return 0;
}

void TestBuildARegion(ChunkBuildPool& pool, Material& mat, int size, ivec3 offset)
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				ivec3 cpos(x, y, z);
				pool.PushShouldGenerate(cpos + offset);
			}
		}
	}
	pool.Generate(size*size*size);

	//std::this_thread::sleep_for(std::chrono::seconds(4));

	/*for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				ivec3 cpos(x, y, z);
				pool.PushShouldMesh(cpos + offset);
			}
		}
	}*/

	// pool.Mesh(size*size*size, mat, ChunkMesh::chunkTriBufferHandle);
}

void TestUnbuildARegion(ChunkBuildPool& pool, int size, ivec3 offset)
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				ivec3 cpos(x, y, z);
				pool.Unload(cpos + offset);
			}
		}
	}
}

void pause()
{
	while (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS );
}

int main(void)
{

	if (initGLFWAndGLEW() < 0)
	{
		return -1;
	}

	TestSomething();

#pragma region setup-shader-etc

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID;
	if (!LoadShaders(
		"shader/TransformVertexShader.glsl",
		"shader/TextureFragmentShader.glsl",
		programID))
	{
		while (1);
	}

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	
	GLuint Texture = loadDDS("shader/uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	GLuint texture = loadDDS("shader/uvtemplate.DDS");

#pragma endregion

	VECam::CamData cam(window);
	VECam::SetupDefaultCam(cam);

	Material mat(programID, texture);

	//static tri indices
	ChunkMesh::InitChunkTriIndices();
	ChunkMesh::LoadChunkTriIndices();

	VEInput::KeyboardDebouncer keyDebouncer(window);


	int trackKeys[]{ 
GLFW_KEY_E,
GLFW_KEY_R,
GLFW_KEY_F,
GLFW_KEY_G,
GLFW_KEY_M,
GLFW_KEY_B,
GLFW_KEY_C,
GLFW_KEY_V,
GLFW_KEY_Y, 
GLFW_KEY_L,};

	keyDebouncer.Track(trackKeys, sizeof(trackKeys));


	ChunkSet cset;

	ChunkBuildPool builder(cset, &TestGetVoxel);
	builder.DEBUGLODLEVEL = NUM_LODS - 1;
	//TestBuildARegion(pool, mat, 3, ivec3(-1, 1, 0));
	//pool.TestLoadDrawableBuffers();

	VEChunkMgmt::ChunkProximity proximity(builder, &providePlayerPos);

	ivec3 buildRegionA(-1, 1, 0);
	ivec3 buildRegionB(4, 1, 0);

	bool testBuild = false;
	int testRegionSize = 3;

	bool usePlayerPosTestMode = true;

	do 
	{
		//
		// Input
		//
		keyDebouncer.CheckKeyPress();
		auto lastKey = keyDebouncer.GetLastKey();
		builder.debugNull = lastKey.keyCode == GLFW_KEY_Y;

		if (!usePlayerPosTestMode)
		{
			if (testBuild || lastKey.keyCode == GLFW_KEY_E)
			{
				testBuild = false;
				TestBuildARegion(builder, mat, testRegionSize, buildRegionA);
			}
			if (lastKey.keyCode == GLFW_KEY_R)
			{
				TestUnbuildARegion(builder, testRegionSize, buildRegionA);
			}

			if (lastKey.keyCode == GLFW_KEY_F)
			{
				TestBuildARegion(builder, mat, testRegionSize, buildRegionB);
			}
			if (lastKey.keyCode == GLFW_KEY_G)
			{
				TestUnbuildARegion(builder, testRegionSize, buildRegionB);
			}
		}
		else
		{
			updateFakePlayerPos(lastKey.keyCode);

			proximity.CurateChunks();
		}
		
		//TEST LODS
		if (lastKey.keyCode == GLFW_KEY_L)
		{
			builder.DEBUGLODLEVEL = (builder.DEBUGLODLEVEL + 1) % NUM_LODS;
		}

		//
		// primitive cpu time management system
		//
		builder.Generate(1);
		builder.Mesh(1, mat, ChunkMesh::chunkTriBufferHandle);


		//
		// Draw commands
		//
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.ComputeMatricesFromInputs();

		builder.TestDrawDrawables(cam);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//drawable.UnloadBuffers();

	builder.TestUnloadDrawableBuffers();

	ChunkMesh::UnloadChunkTriIndices();

	return 0;
}
