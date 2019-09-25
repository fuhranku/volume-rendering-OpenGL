#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// Vertex data out data
out vec3 vColor;

void main()
{
    vColor = vertexPosition + 0.5f;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
}