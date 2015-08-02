#version 330 core

// Include the Ashima code here!

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

out vec2 vUv;

void main() 
{
    vUv = uv;
    gl_Position = MVP * vec4( position, 1.0 );
}
