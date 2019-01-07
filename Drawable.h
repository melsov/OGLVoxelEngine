#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Material.h"


class Drawable
{
public:

	//Drawable();
	Drawable(glm::mat4& _MVP, Material _mat) :
		MVP(_MVP),
		mat(_mat) {}

	~Drawable();
	void Debug();
	void LoadBuffers();
	void Draw();

	std::vector<GLfloat>* verts;
	std::vector<GLfloat>* uvs;
	std::vector<unsigned short>* indices;
	Material mat;

	glm::mat4& MVP;

};

