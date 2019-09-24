#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec2 texCoords;

// Texture 
uniform sampler2D renderedTexture;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(vec3(texture(renderedTexture,texCoords).rgb),1.0f);
}