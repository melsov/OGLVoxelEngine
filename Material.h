#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Material
{
public:

	/*const char* vertFilePath;
	const char* fragFilePath;*/

	Material();
	Material(GLuint _programID, GLuint texture); 
	~Material();
	void UseProgram();
	void SetTransform(const glm::mat4& MVP);
	void SetLODScale(float scale = 1);
	void BindTexture();
	//void Bind(GLuint vertexBufferHandle, GLuint uvBufferHandle, GLuint trisBufferHandle, int offset = 0);
	void Bind(GLuint vvertBufferHandle, GLuint trisBufferHandle, int stride, int offset = 0);
	void Disable();


private:

	GLuint programID;
	GLint matrixID;
	GLint textureID;
	GLuint texture;
	GLuint scaleID;

};

