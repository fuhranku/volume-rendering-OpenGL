#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec3 vertexColor;
// Atribute 2 of the vertex
layout (location = 2) in vec2 uvCoords;

// Vertex data out data
out vec3 vColor;
out vec2 texCoords;

void main()
{
    vColor = vertexColor;
	texCoords = uvCoords;
    gl_Position = vec4(vertexPosition, 1.0f);
}