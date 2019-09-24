#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;

// Texture 
uniform sampler3D volTexture;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(vec3(vColor),1.0f);
}