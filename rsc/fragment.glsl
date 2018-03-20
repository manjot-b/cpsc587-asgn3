#version 410 core
out vec4 fragColor;
in vec4 vertexColor;	// used for input from vertex shader
uniform vec4 uColor;			// used for input from program (CPU)
uniform int drawPoints = 0;		// used for debugging. Will change color of just points
uniform float primitive = 0.0;		// used for debugging. Will change colour of different primitives

void main()
{
	fragColor = uColor;
	// fragColor = vec4(1, 0.9, 0, 1.0);

	// if (drawPoints == 1)
	// 	fragColor = vec4(1, 1, 1, 1);
}