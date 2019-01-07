#include "Drawable.h"
#include <stdio.h>


Drawable::~Drawable()
{
	printf("drawable destructor\n");
}

void Drawable::Debug() 
{
	printf("size of verts: %d \n", verts->size());
}

void Drawable::LoadBuffers()
{
	//mat.LoadVertexBuffer(verts);
	//mat.LoadUVBuffer(uvs);
	//mat.LoadTriIndicesBuffer(indices);

	//TODO: update if we need this
	// Drawable assume non static tri buffers and
	// works with material the old way
}

void Drawable::Draw()
{
/*
	mat.UseProgram();
	mat.SetTransform(MVP);
	//mat.Bind(); // TODO: update drawable so that it owns vert, uv and tri handles

	glDrawElements(
		GL_TRIANGLES,
		indices->size(),
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	mat.Disable();
*/
}
