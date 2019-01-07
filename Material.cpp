#include "Material.h"

#include "shader.hpp"
#include "texture.hpp"


enum BufferLayoutLocation
{
	VERT = 0,
	UV = 1, 
	COLOR = 2,
};


Material::Material()
{
}


Material::Material(GLuint _programID, GLuint _texture) 
{
	programID = _programID;
	texture = _texture; 
	textureID = glGetUniformLocation(programID, "myTextureSampler");
	matrixID = glGetUniformLocation(programID, "MVP");
}

void Material::UseProgram()
{
	glUseProgram(programID);
}

void Material::SetTransform(const glm::mat4& MVP)
{
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
}

void Material::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture to user texture unit 0
	glUniform1i(textureID, 0);
}

void bindBuffer(int location, int size, GLuint handle, int gl_type, int offset = 0, int stride = 0)
{
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glVertexAttribPointer(
		location, // must match location in shader
		size,     // number of elements (e.g. 3 for a vec3 position)
		gl_type, // type
		GL_FALSE, // normalized?
		stride,        // stride
		(void *)offset // array buffer offset
	);
}

//void Material::Bind(GLuint vertexBufferHandle, GLuint uvBufferHandle, GLuint trisBufferHandle, int offset)
//{
//	bindBuffer(VERT, 3, vertexBufferHandle, GL_FLOAT, offset);
//	bindBuffer(UV, 2, uvBufferHandle, GL_FLOAT, offset);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trisBufferHandle);
//}

void Material::Bind(GLuint vvertBufferHandle, GLuint trisBufferHandle, int stride, int offset)
{
	bindBuffer(VERT, 3, vvertBufferHandle, GL_FLOAT, offset, stride);
	bindBuffer(UV, 2, vvertBufferHandle, GL_FLOAT, offset + 3 * sizeof(GLfloat), stride);
	bindBuffer(COLOR, 4, vvertBufferHandle, GL_FLOAT, offset + 5 * sizeof(GLfloat), stride);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trisBufferHandle);

}


void Material::Disable()
{
	glDisableVertexAttribArray(VERT);
	glDisableVertexAttribArray(UV);
}



Material::~Material()
{
	
}
