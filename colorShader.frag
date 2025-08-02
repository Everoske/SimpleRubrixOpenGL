#version 330 core
out vec4 FragColor;

uniform vec3 cubeColor;

void main()
{
	// Front
	if (gl_PrimitiveID < 2)
		FragColor = vec4(vec3(0.0, 1.0, 0.0), 1.0);
	// Right
	else if (gl_PrimitiveID < 4)
		FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
	// Left
	else if (gl_PrimitiveID < 6)
		FragColor = vec4(vec3(1.0, 0.5, 0.0), 1.0);
	// Top
	else if (gl_PrimitiveID < 8)
		FragColor = vec4(vec3(1.0, 1.0, 1.0), 1.0);
	// Bottom
	else if (gl_PrimitiveID < 10)
		FragColor = vec4(vec3(1.0, 1.0, 0.0), 1.0);
	// Back
	else if (gl_PrimitiveID < 12)
		FragColor = vec4(vec3(0.0, 0.0, 1.0), 1.0);
	else
		FragColor = vec4(vec3(0.25, 0.25, 0.25), 1.0);
}