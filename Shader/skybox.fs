#version 150
#extension GL_ARB_explicit_attrib_location : require

in vec3	Color;

out vec4	out_Color;

uniform samplerCube	CubemapTexture;

void	main()
{

	out_Color	=	texture(CubemapTexture, Color);
}
