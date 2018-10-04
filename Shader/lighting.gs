// GLSL version
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;
in VertexAttrib
{
vec2 CoordTexture;
vec3 WorldPos0;
vec3 Normal0;
}vertex[];

out vec2 CoordTexture;
out vec3 WorldPos0;
out vec3 Normal0;

void main()
{
    for (int i=0; i< 3; ++i)
    {
        gl_Position     = gl_in[i].gl_Position;

        CoordTexture    =   vertex[i].CoordTexture;
        WorldPos0       =   vertex[i].WorldPos0;
        Normal0         =   vertex[i].Normal0;

        EmitVertex();
    }
    EndPrimitive();
}

