#version 330 core
layout(location = 0) out vec3 color;
// Texture 
uniform sampler3D volTexture;
uniform float step;

in vec3 vColor;

void main()
{
    color = vColor;
}