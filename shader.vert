#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 partPosition;
layout (location = 2) in vec4 color;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;
out vec4 col;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
	float pSize = partPosition.w;
	vec3 particleCenter = partPosition.xyz;
	vec3 vertexPosition = particleCenter 
						+ cameraRight * position.x * pSize; 
						+ cameraUp * position.y * pSize;

    gl_Position = projection * view * vec4(position + partPosition.xyz, 1.0);
    sampleExtraOutput = 1.0f;
	col = color;
}