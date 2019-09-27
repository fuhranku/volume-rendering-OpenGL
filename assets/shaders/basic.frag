#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec2 texCoords;

// Texture 
uniform sampler3D volTexture;
uniform float step;

// Fragment Color
out vec4 color;

void main()
{
	vec3 tex = vec3(texCoords.x,texCoords.y,step);
    color = vec4(vec3(texture(volTexture,tex).r),1.0f);
	//color = vec4(vColor,1.0f);
}