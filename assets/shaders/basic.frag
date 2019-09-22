#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec2 texCoords;

// Texture 
uniform sampler3D volTexture;

// Fragment Color
out vec4 color;

void main()
{
	vec3 tex = vec3(texCoords,0.5f/256);
    color = texture3D(volTexture,tex);
}