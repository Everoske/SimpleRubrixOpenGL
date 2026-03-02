#version 330 core
out vec4 FragColor;

uniform vec3 frontColor;
uniform vec3 rightColor;
uniform vec3 leftColor;
uniform vec3 topColor;
uniform vec3 bottomColor;
uniform vec3 backColor;

uniform int highlight;

void main()
{
	// Front
	if (gl_PrimitiveID < 2)
	{
		if (highlight > 0)
		{
			vec3 hFrontColor = vec3(min(frontColor.x * 1.2, 1.0),
			                        min(frontColor.y * 1.2, 1.0),
			                        min(frontColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hFrontColor, 1.0);
		}
		else
		{
			FragColor = vec4(frontColor, 1.0);
		}
	}
	// Right
	else if (gl_PrimitiveID < 4)
	{
		if (highlight > 0)
		{
			vec3 hRightColor = vec3(min(rightColor.x * 1.2, 1.0),
			                        min(rightColor.y * 1.2, 1.0),
			                        min(rightColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hRightColor, 1.0);
		}
		else
		{
			FragColor = vec4(rightColor, 1.0);
		}
	}
	// Left
	else if (gl_PrimitiveID < 6)
	{
		if (highlight > 0)
		{
			vec3 hLeftColor = vec3(min(leftColor.x * 1.2, 1.0),
			                        min(leftColor.y * 1.2, 1.0),
			                        min(leftColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hLeftColor, 1.0);
		}
		else
		{
			FragColor = vec4(leftColor, 1.0);
		}
	}
	// Top
	else if (gl_PrimitiveID < 8)
	{
		if (highlight > 0)
		{
			vec3 hTopColor = vec3(min(topColor.x * 1.2, 1.0),
			                        min(topColor.y * 1.2, 1.0),
			                        min(topColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hTopColor, 1.0);
		}
		else
		{
			FragColor = vec4(topColor, 1.0);
		}
	}
	// Bottom
	else if (gl_PrimitiveID < 10)
	{
		if (highlight > 0)
		{
			vec3 hBottomColor = vec3(min(bottomColor.x * 1.2, 1.0),
			                        min(bottomColor.y * 1.2, 1.0),
			                        min(bottomColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hBottomColor, 1.0);
		}
		else
		{
			FragColor = vec4(bottomColor, 1.0);
		}
	}
	// Back
	else if (gl_PrimitiveID < 12)
	{
		if (highlight > 0)
		{
			vec3 hBackColor = vec3(min(backColor.x * 1.2, 1.0),
			                        min(backColor.y * 1.2, 1.0),
			                        min(backColor.z * 1.2, 1.0)
									);
			FragColor = vec4(hBackColor, 1.0);
		}
		else
		{
			FragColor = vec4(backColor, 1.0);
		}
	}
}