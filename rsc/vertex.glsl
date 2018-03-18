#version 410 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
out vec4 vertexColor;
uniform mat4 projectionView;


void main()
{
	// gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projectionView * vec4(aPos, 1.0);
	vertexColor = vec4(aPos, 1.0);
}
