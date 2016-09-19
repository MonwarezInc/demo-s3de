#version 330

layout (location = 0) in vec3 	position;

uniform	mat4	WVP;

out	vec3	Color;

void main()
{
	vec4	pos	=	WVP * vec4(position,1.0);
	gl_Position	=	pos.xyww;
	Color		=	pos;
}
