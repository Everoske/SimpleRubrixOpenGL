#version 330 core
out vec4 FragColor;

uniform vec3 frontColor;
uniform vec3 rightColor;
uniform vec3 leftColor;
uniform vec3 topColor;
uniform vec3 bottomColor;
uniform vec3 backColor;

void main()
{
	// Front
	if (gl_PrimitiveID < 2)
		// FragColor = vec4(vec3(0.0, 1.0, 0.0), 1.0);
		FragColor = vec4(frontColor, 1.0);
	// Right
	else if (gl_PrimitiveID < 4)
		// FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
		FragColor = vec4(rightColor, 1.0);
	// Left
	else if (gl_PrimitiveID < 6)
		// FragColor = vec4(vec3(1.0, 0.5, 0.0), 1.0);
		FragColor = vec4(leftColor, 1.0);
	// Top
	else if (gl_PrimitiveID < 8)
		// FragColor = vec4(vec3(1.0, 1.0, 1.0), 1.0);
		FragColor = vec4(topColor, 1.0);
	// Bottom
	else if (gl_PrimitiveID < 10)
		// FragColor = vec4(vec3(1.0, 1.0, 0.0), 1.0);
		FragColor = vec4(bottomColor, 1.0);
	// Back
	else if (gl_PrimitiveID < 12)
		// FragColor = vec4(vec3(0.0, 0.0, 1.0), 1.0);
		FragColor = vec4(backColor, 1.0);
}