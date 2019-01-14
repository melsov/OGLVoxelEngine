#include "DrawableChunk.h"
#include <stdio.h>




void DrawableChunk::Debug()
{
	printf("size of verts: %d \n", md->verts.size()); // verts(0)->size());
}



//GLuint loadBuffer(const std::vector<GLfloat>* buff)
//{
//	GLuint handle;
//	glGenBuffers(1, &handle);
//	glBindBuffer(GL_ARRAY_BUFFER, handle);
//	glBufferData(GL_ARRAY_BUFFER, buff->size() * sizeof(GLfloat), &(*buff)[0], GL_STATIC_DRAW);
//	return handle;
//}

//GLuint loadBuffer(const std::vector<GLfloat>* buff)
//{
//	GLuint handle;
//	glGenBuffers(1, &handle);
//	glBindBuffer(GL_ARRAY_BUFFER, handle);
//	glBufferData(GL_ARRAY_BUFFER, buff->size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, buff->size() * sizeof(GLfloat), &(*buff)[0]); // OK this works
//	return handle;
//}

GLuint loadBuffer(const GLfloat* buff, GLuint size)
{
	GLuint handle;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, buff); // &(*buff)[0]); // OK this works
	return handle;
}

void DrawableChunk::LoadBuffers()
{
	if (loaded || verts(0)->size() == 0) // verts->size() == 0)
	{
		return;
	}
	loaded = true;
	
	auto m = *md;
	//handles.verts = loadBuffer(&m.verts);
	handles.verts = loadBuffer(m.verts[0].pos, m.verts.size() * sizeof(ChunkMesh::VVertex));
	//handles.uvs = loadBuffer(&m.uvs);

	// TODO: does the number of verts / 4 * 6 (i.e. num tri indices)
	// exceed the length of our (static) tri index buffer?
	// if so, do something to handle this situation
	// perhaps split into two (or more) arrays / buffers

	//mat.Bind(handles.verts, handles.uvs, handles.tris);
	mat.Bind(handles.verts, handles.tris, sizeof(ChunkMesh::VVertex), 0);
}

void DrawableChunk::UnloadBuffers()
{
	mat.Disable();
	for (int i = 0; i < NUM_LODS; ++i)
	{
		auto m = *md; // md->mds[i];
		glDeleteBuffers(1, &handles.verts);
		//glDeleteBuffers(1, &handles.uvs);
	}
	loaded = false;
}

//TODO: choose lod func
void DrawableChunk::Draw(glm::mat4 View, glm::mat4 Projection, int LOD)
{
	mat.UseProgram();
	mat.SetTransform(Projection * View * ModelMatrix);
	mat.SetLODScale(pow(2, LOD));

	mat.BindTexture();
	int lod = 0; //TODO

	mat.Bind(handles.verts, handles.tris, sizeof(ChunkMesh::VVertex), 0);
	GLsizei howMany = md->triCountAtLOD(LOD);

	glDrawElements(
		GL_TRIANGLES,
		howMany,
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	mat.Disable();
}
