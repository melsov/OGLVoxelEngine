#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 COLOR_;

// Ouput data
out vec3 color;


// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	// Output color = color of the texture at the specified UV
	// color = texture( myTextureSampler, UV ).rgb * COLOR_.xyz;



	vec2 muv = mod(UV, .33);
	vec2 dbugFacePos = min( muv, abs(muv - .33));
	float dist = length(dbugFacePos);
	// if(dist < .1)
	// 	color.xyz = COLOR_.xyz;
	// else 
	// 	color.xyz = texture(myTextureSampler, UV).rgb;

	color = texture( myTextureSampler, UV ).rgb * COLOR_.xyz;
}