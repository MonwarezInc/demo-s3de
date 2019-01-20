#version 330

in vec3	Color;

out vec4	out_Color;

uniform samplerCube	CubemapTexture;

void	main()
{

	out_Color	=	texture(CubemapTexture, Color);
}
