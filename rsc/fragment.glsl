#version 410 core
out vec4 fragColor;
in vec4 vertexColor;	// used for input from vertex shader
uniform vec4 uColor;			// used for input from program (CPU)
uniform float drawPoints = 0.0;		// used for debugging. Will change color of just points
uniform float primitive = 0.0;		// used for debugging. Will change colour of different primitives

void main()
{
	// fragColor = vec4(0.5, drawPoints, primitive, 1.0);	// if line- red, if point - green. else purple
	fragColor = vertexColor;

	if (drawPoints == 1.0)
		fragColor = vec4(0, 1, 0, 1);
}