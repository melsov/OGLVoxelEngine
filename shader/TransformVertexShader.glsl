#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 color;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 COLOR_;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// Scale for LOD levels
uniform float scale;

void main()
{

	vec3 pos = vertexPosition_modelspace;

	//LOD adjust pos
	vec3 m = mod(pos, 1);
	pos = pos - mod(pos, scale) + floor(m + .49) * scale;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(pos,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
	COLOR_ = color;
}

